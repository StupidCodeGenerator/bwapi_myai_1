#include "ResourceManager.h"
#include <BWAPI.h>
#include "MyAiModule.h"
#include "utils.h"
#include "MineralExtend.h"

int ResourceManager::OnInit() {
	return FUCK_SUCCESS;
}


int ResourceManager::OnUpdate() {

	std::list<int> deadList;
	deadList.clear();

	// 去掉那些找不到的Unit
	std::map<int, MineralExtend*>::iterator fuckIT = m_MineralsMap.begin();
	for (fuckIT; fuckIT != m_MineralsMap.end(); fuckIT++) {
		if (!fuckIT->second) {
			deadList.push_back(fuckIT->first);
			continue;
		}
		else {
			BWAPI::Unit unit = AI_MODULE->FindMinerals(fuckIT->second->m_UnitID);
			if (!unit) {
				deadList.push_back(fuckIT->first);
				SAFE_DELETE(fuckIT->second);   // 顺便给second释放了
				continue;
			}
		}
	}
	std::list<int>::iterator shitIT = deadList.begin();
	for (shitIT; shitIT != deadList.end(); shitIT++) {
		m_MineralsMap.erase(*shitIT);
	}

	return FUCK_SUCCESS;
}


int ResourceManager::CreateMineralExtend(BWAPI::Unit unit) {
	if (!unit || !unit->exists()) {
		return FUCK_ERR__NULL_UNIT;
	}

	if (m_MineralsMap.find(unit->getID()) != m_MineralsMap.end()) {
		return FUCK_ERR__UNIT_ALREADY_IN;
	}

	MineralExtend* pFuck = new MineralExtend();
	pFuck->OnInit(unit->getID());

	m_MineralsMap.insert(make_pair(unit->getID(), pFuck));

	return FUCK_SUCCESS;
}



// 直接循环查找, 排鸡毛排序
// 最后返回的是UnitID
int ResourceManager::FindMineralUnitID_of_MinSCV(int maxDistance, BWAPI::Unit scvUnit) {

	// 1. 首先找到一个集合, 包含所有SCV数量最小的集合.
	std::list<int> minSCV_Set;
	minSCV_Set.clear();
	int minValue = 99999;
	std::map<int, MineralExtend*>::iterator fuckIT = m_MineralsMap.begin();
	for (fuckIT; fuckIT != m_MineralsMap.end(); fuckIT++) {
		int value = fuckIT->second->GetSCVCount();
		if (value < minValue) {
			minValue = value;
			minSCV_Set.clear();
			minSCV_Set.push_back(fuckIT->first);
		}
		else if (value == minValue) {
			minSCV_Set.push_back(fuckIT->first);
		}
	}

	// 2. 找到这个集合后, 如果集合Count为1, 直接返回, 如果小于1返回错误
	if (minSCV_Set.size() < 1) {
		return NULL_UNIT_ID;
	}
	else if (minSCV_Set.size() == 1) {
		return *(minSCV_Set.begin());
	}

	// 3. 对于minSCV_Set.size() > 1 的情况, 要找到距离自己最近的
	int minDistance = 99999;
	int finalUnit = NULL_UNIT_ID;
	std::list<int>::iterator shitIT = minSCV_Set.begin();
	for (shitIT; shitIT != minSCV_Set.end(); shitIT++) {
		BWAPI::Unit mineralObj = AI_MODULE->FindMinerals(*shitIT);
		if (!mineralObj) {
			continue;
		}
		int distance = scvUnit->getDistance(mineralObj);
		if (distance < minDistance) {
			minDistance = distance;
			finalUnit = mineralObj->getID();
		}
	}

	return finalUnit;
}


std::list<int> ResourceManager::FindMineralUnitInRange(BWAPI::Position center, int radius) {
	std::list<int> output;
	std::map<int, MineralExtend*>::iterator fuckIT = m_MineralsMap.begin();
	for (fuckIT; fuckIT != m_MineralsMap.end(); fuckIT++) {
		BWAPI::Unit unit = FIND_UNIT(fuckIT->second->m_UnitID);
		if (unit && unit->getDistance(center) <= radius) {
			output.push_back(unit->getID());
		}
	}

	return output;
}

std::list<int> ResourceManager::FindGasUnitInRange(BWAPI::Position center, int radius) {
	std::list<int> output;
	for (auto &u : BWAPI::Broodwar->getAllUnits()) {
		if (u && u->getType() == BWAPI::UnitTypes::Resource_Vespene_Geyser && u->getDistance(center) < radius) {
			output.push_back(u->getID());
		}
	}

	return output;
}


int ResourceManager::AddSCV_of_Mineral(int mineralUnitID) {
	std::map<int, MineralExtend*>::iterator fuckIT = m_MineralsMap.find(mineralUnitID);
	if (fuckIT == m_MineralsMap.end() || !fuckIT->second) {
		return FUCK_ERR__NULL_UNIT;
	}
	fuckIT->second->AddSCV();
	return FUCK_SUCCESS;
}


int ResourceManager::RemoveSCV_of_Mineral(int mineralUnitID) {
	std::map<int, MineralExtend*>::iterator fuckIT = m_MineralsMap.find(mineralUnitID);
	if (fuckIT == m_MineralsMap.end() || !fuckIT->second) {
		return FUCK_ERR__NULL_UNIT;
	}
	fuckIT->second->RemoveSCV();
	return FUCK_SUCCESS;
}


void ResourceManager::OnDraw() {
	std::map<int, MineralExtend*>::iterator fuckIT = m_MineralsMap.begin();
	for (fuckIT; fuckIT != m_MineralsMap.end(); fuckIT++) {
		BWAPI::Unit u = FIND_UNIT(fuckIT->second->m_UnitID);
		if (!u) {
			continue;
		}
		BWAPI::Broodwar->drawCircleMap(u->getPosition(), 30, BWAPI::Colors::White);
	}
}
