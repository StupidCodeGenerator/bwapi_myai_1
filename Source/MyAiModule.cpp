#include <iostream>
#include <Windows.h>
#include "MyAiModule.h"
//#include "BackManager.h"
//#include "FrontManager.h"
//#include "InformationManager.h"

#include "UnitBrainManager.h"
#include "UnitBrain.h"

#include "utils.h"

#define SELF Broodwar->self()

MyAiModule* AI_MODULE = NULL;

using namespace BWAPI;
using namespace Filter;
using namespace std;




BWAPI::Unit MyAiModule::FindUnit(int unitID) {
	std::map<int, BWAPI::Unit>::iterator fuckIT = m_UnitMap.find(unitID);
	if (fuckIT == m_UnitMap.end()) {
		return NULL;
	}
	else {
		return fuckIT->second;
	}
}

int MyAiModule::GetMoney() {
	return SELF->minerals();
}

int MyAiModule::GetGas() {
	return SELF->gas();
}

int MyAiModule::GetSupply() {
	return (SELF->supplyTotal() - SELF->supplyUsed()) / 2;       // because of the zerglings
}

int MyAiModule::GetUnitCount(BWAPI::UnitType unitType) {
	int output = 0;
	std::map<int, Unit>::iterator fuckIT = m_UnitMap.begin();
	for (fuckIT; fuckIT != m_UnitMap.end(); fuckIT++) {
		if (fuckIT->second && fuckIT->second->getType() == unitType) {
			output++;
		}
	}

	return output;
}


int MyAiModule::OnUpdate() {

	if (m_pUnitBrainManager) {
		m_pUnitBrainManager->OnUpdate();
	}

	return FUCK_SUCCESS;
}



void MyAiModule::onStart() {
	
	AI_MODULE = this;
	
	this->m_pUnitBrainManager = new UnitBrainManager();
	this->m_pUnitBrainManager->OnInit();

	// Enable the UserInput flag, which allows us to control the bot and type messages.
	Broodwar->enableFlag(Flag::UserInput);

	onSendText("MyAiModule::onStart");
}

void MyAiModule::onEnd(bool isWinner) {
	onSendText("MyAiModule::onEnd");
}

void MyAiModule::onFrame() {

	DWORD nowTime = GetTickCount();
	if (nowTime - m_LastUpdateTime > 100) {  // 限制Update的时间, 别太快
		OnUpdate();
		m_LastUpdateTime = nowTime;
	}

	LogOnScreen();

	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

}

void MyAiModule::onSendText(std::string text) {
	// Send the text to the game if it is not being processed.
	Broodwar->sendText("%s", text.c_str());
}

void MyAiModule::onReceiveText(BWAPI::Player player, std::string text) {
	onSendText("MyAiModule::onReceiveText " + text);
}

void MyAiModule::onPlayerLeft(BWAPI::Player player) {
	onSendText("MyAiModule::onPlayerLeft");
}

void MyAiModule::onNukeDetect(BWAPI::Position target) {
	onSendText("MyAiModule::onNukeDetect");
}

void MyAiModule::onUnitDiscover(BWAPI::Unit unit) {
	onSendText("MyAiModule::onUnitDiscover ");
}

void MyAiModule::onUnitEvade(BWAPI::Unit unit) {
//	onSendText("MyAiModule::onUnitEvade ");
}

// 某单位出现,包括己方单位,敌方单位
void MyAiModule::onUnitShow(BWAPI::Unit unit) {
//	onSendText("MyAiModule::onUnitShow ");
}

void MyAiModule::onUnitHide(BWAPI::Unit unit) {
//	onSendText("MyAiModule::onUnitHide ");
}

// 某单位被创建, 在建造中的也叫创建, 包括在兵营中正在读条的枪兵.
void MyAiModule::onUnitCreate(BWAPI::Unit unit) {
	onSendText("MyAiModule::onUnitCreate ");
	if (unit && unit->getPlayer()->getID() == SELF->getID()) {
		m_UnitMap.insert(make_pair(unit->getID(), unit));
		if (m_pUnitBrainManager) {
			m_pUnitBrainManager->CreateBrain(unit);
		}
	}
}

void MyAiModule::onUnitDestroy(BWAPI::Unit unit) {
	onSendText("MyAiModule::onUnitDestroy ");
	if (m_UnitMap.find(unit->getID()) != m_UnitMap.end()) {
		m_UnitMap.erase(unit->getID());
		// the brain will auto remove in update
	}
}

void MyAiModule::onUnitMorph(BWAPI::Unit unit) {
//	onSendText("MyAiModule::onUnitMorph ");
}

void MyAiModule::onUnitRenegade(BWAPI::Unit unit) {
	onSendText("MyAiModule::onUnitRenegade ");
}

void MyAiModule::onSaveGame(std::string gameName) {
}

// 表示单位是否建造结束, 并且,Unit本身也有 isComplete() 判定函数
void MyAiModule::onUnitComplete(BWAPI::Unit unit) {
	onSendText("MyAiModule::onUnitComplete ");
}


void MyAiModule::LogOnScreen() {
	if (m_pUnitBrainManager)
	{
		Broodwar->drawTextScreen(0, 0, "%d / %d / %d", m_UnitMap.size(), SELF->getUnits().size(), m_pUnitBrainManager->m_UnitBrainMap.size());
	}
}	