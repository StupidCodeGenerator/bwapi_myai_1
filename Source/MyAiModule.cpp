#include <iostream>
#include "MyAiModule.h"
#include "BackManager.h"
#include "FrontManager.h"
#include "InformationManager.h"

using namespace BWAPI;
using namespace Filter;


void MyAiModule::onStart() {
	// Enable the UserInput flag, which allows us to control the bot and type messages.
	Broodwar->enableFlag(Flag::UserInput);
	
	// Managers
	BackManager::GetInstance()->OnStart();
	FrontManager::GetInstance()->OnStart();
	InformationManager::GetInstance()->OnStart();
}

void MyAiModule::onEnd(bool isWinner) {
}

void MyAiModule::onFrame() {

	LogOnScreen();

	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	// Managers
	BackManager::GetInstance()->OnFrame();
	FrontManager::GetInstance()->OnFrame();
	InformationManager::GetInstance()->OnFrame();
}

void MyAiModule::onSendText(std::string text) {
	// Send the text to the game if it is not being processed.
	Broodwar->sendText("%s", text.c_str());
}

void MyAiModule::onReceiveText(BWAPI::Player player, std::string text) {
}

void MyAiModule::onPlayerLeft(BWAPI::Player player) {
}

void MyAiModule::onNukeDetect(BWAPI::Position target) {
}

void MyAiModule::onUnitDiscover(BWAPI::Unit unit) {
}

void MyAiModule::onUnitEvade(BWAPI::Unit unit) {
}

void MyAiModule::onUnitShow(BWAPI::Unit unit) {
}

void MyAiModule::onUnitHide(BWAPI::Unit unit) {
}

void MyAiModule::onUnitCreate(BWAPI::Unit unit) {
}

void MyAiModule::onUnitDestroy(BWAPI::Unit unit) {
	FrontManager::GetInstance()->OnUnitDestory(unit);
	InformationManager::GetInstance()->OnUnitDestory(unit);
}

void MyAiModule::onUnitMorph(BWAPI::Unit unit) {
	BackManager::GetInstance()->OnUnitMorph(unit);
	FrontManager::GetInstance()->OnUnitMorph(unit);
	InformationManager::GetInstance()->OnUnitMorph(unit);
}

void MyAiModule::onUnitRenegade(BWAPI::Unit unit) {
	//Broodwar->sendText("UnitRenegade: %s", unit->getType().c_str());
}

void MyAiModule::onSaveGame(std::string gameName) {
}

void MyAiModule::onUnitComplete(BWAPI::Unit unit) {
	BackManager::GetInstance()->OnUnitComplete(unit);
	InformationManager::GetInstance()->OnUnitComplete(unit);
}




void MyAiModule::LogOnScreen() {
	// output

	double screenX = Broodwar->getScreenPosition().x;
	double screenY = Broodwar->getScreenPosition().y;

	Broodwar->drawTextScreen(0, 0, "BackManager.state = %s", BackManager::GetInstance()->GetStateStr());
	Broodwar->drawTextScreen(0, 10, "FrontManager.state = %s", FrontManager::GetInstance()->GetStateStr());
	Broodwar->drawTextScreen(0, 20, "InformationManager.state = %s", InformationManager::GetInstance()->GetStateStr());

	if (InformationManager::GetInstance()->pEnemyTilePosition) {
		Broodwar->drawTextScreen(0, 30, "EnemyPoint : (%d, %d)",
			InformationManager::GetInstance()->pEnemyTilePosition->x, InformationManager::GetInstance()->pEnemyTilePosition->y);
	}

	Broodwar->drawTextScreen(0, 40, "AttackLimit=%d", FrontManager::GetInstance()->attackLimit);
	Broodwar->drawTextScreen(0, 50, "Lings = %d", FrontManager::GetInstance()->lingCount);
	Broodwar->drawTextScreen(0, 60, "SupplyTotal = %d", BackManager::GetInstance()->m_SupplyTotal);

	Broodwar->drawTextScreen(500, 30, "Plan:");
	if (!BackManager::GetInstance()->m_PlanList.empty()) {
		int y = 40;
		for (auto &p : BackManager::GetInstance()->m_PlanList) {
			Broodwar->drawTextScreen(500, y, "Type=%s", p->type.c_str());
			y += 10;
		}
	}

	Broodwar->drawTextScreen(400, 30, "Progress:");
	if (!BackManager::GetInstance()->m_Progress.empty()) {
		int y = 40;
		for (auto &p : BackManager::GetInstance()->m_Progress) {
			Broodwar->drawTextScreen(400, y, "Type=%s", p->type.c_str());
			y += 10;
		}
	}

	Broodwar->drawTextScreen(200, 0, "UnderAttackTimeCount=%d", FrontManager::GetInstance()->underAttackTimeCount);

	for (auto &u : Broodwar->self()->getUnits()) {
		if (u->getType() == UnitTypes::Zerg_Egg) {
			if (!u->getTrainingQueue().empty()) {
				UnitType type = u->getTrainingQueue().front();
				Broodwar->drawTextScreen(u->getPosition().x - screenX, u->getPosition().y - screenY, type.c_str());
			}
		} else if (u->getType().isBuilding() && u->isBeingConstructed()) {
			Broodwar->drawTextScreen(u->getPosition().x - screenX, u->getPosition().y - screenY, "Constructing: %s", u->getType().c_str());
		} else if (u->getType().isWorker() && u->isIdle()) {
			Broodwar->drawTextScreen(u->getPosition().x - screenX, u->getPosition().y - screenY, "idle");
		}
	}

	int count = 0;
}