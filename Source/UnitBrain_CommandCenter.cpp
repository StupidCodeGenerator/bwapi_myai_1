#include "UnitBrain_CommandCenter.h"
#include "utils.h"
#include <BWAPI.h>
#include "MyAiModule.h"
#include "ResourceManager.h"

int UnitBrain_CommandCenter::GetUnitID() {
	return this->m_UnitID;
}

int UnitBrain_CommandCenter::OnInit(int unitID) {
	this->m_UnitID = unitID;

	BWAPI::Unit unit = FIND_UNIT(unitID);
	if (!unit) {
		return FUCK_ERR__NULL_UNIT;
	}

	RefreshBuildableTiles();

	return FUCK_SUCCESS;
}

// 凡是查找关于矿和建筑位置信息的时候, 统统都先刷一遍
// 因为Buildable会一直变化, 采矿,建筑物等等都影响
// 初始化的时候刷也不行, 因为有可能其他Unit没刷出来呢
void UnitBrain_CommandCenter::RefreshBuildableTiles() {

	AI_MODULE->onSendText("UnitBrain_CommandCenter::RefreshBuildableTiles");

	if (!AI_MODULE || !AI_MODULE->m_pResourceManager) {
		return;
	}

	BWAPI::Unit unit = FIND_UNIT(this->m_UnitID);
	// 查找所有的矿
	m_Minerals = AI_MODULE->m_pResourceManager->FindMineralUnitInRange(unit->getPosition(), COMMAND_CENTER_RANGE);

	// 查找所有的气
	m_GasStations = AI_MODULE->m_pResourceManager->FindGasUnitInRange(unit->getPosition(), COMMAND_CENTER_RANGE);

	// 查找所有的可建造地图块	
	TileEx tileSeed;
	tileSeed.OnInit(unit->getTilePosition());
	ExtendBuildableTiles_Recursive(tileSeed, unit);

	// 去掉距离矿太近的以及距离CommandCenter太近的地图块
	std::list<int> toRemove;
	toRemove.clear();
	std::map<int, TileEx>::iterator tileIT = m_BuildableTilesMap.begin();
	for (tileIT; tileIT != m_BuildableTilesMap.end(); tileIT++) {
		if (tileIT->second.DistanceToUnit(unit) < COMMAND_CENTER_MIN_BUILD_DISTANCE) {
			toRemove.push_back(tileIT->first);
		}
		else {
			std::list<int>::iterator mineralIT = m_Minerals.begin();
			for (mineralIT; mineralIT != m_Minerals.end(); mineralIT++) {
				BWAPI::Unit mineralUnit = FIND_UNIT(*mineralIT);
				if (!mineralUnit) {
					continue;
				}

				if (tileIT->second.DistanceToUnit(mineralUnit) < COMMAND_CENTER_MIN_BUILD_DISTANCE) {
					toRemove.push_back(tileIT->first);
				}
			}

			std::list<int>::iterator gasIT = m_GasStations.begin();
			for (gasIT; gasIT != m_GasStations.end(); gasIT++) {
				BWAPI::Unit gasUnit = FIND_UNIT(*gasIT);
				if (gasUnit && tileIT->second.DistanceToUnit(gasUnit) < COMMAND_CENTER_MIN_BUILD_DISTANCE) {
					toRemove.push_back(tileIT->first);
				}
			}
		}
	}

	// 然后需要去掉SCV采矿路线上的地图块
	// TODO 

	std::list<int>::iterator removeIT = toRemove.begin();
	for (removeIT; removeIT != toRemove.end(); removeIT++) {
		m_BuildableTilesMap.erase(*removeIT);
	}
}


//
//	OK, 一阶逻辑: 
//	获得SCV总数, CommandCenter总数, 以及人口上限, 金钱
//	如果金钱 > 50, 人口上限未满, SCV总数 / CommandCenter总数 < 15, 并且处于空闲状态, 则造农民
//
int UnitBrain_CommandCenter::OnUpdate() {

	if (!AI_MODULE) {
		return FUCK_ERR__NULL_AI_MODULE;
	}

	BWAPI::Unit unit = FIND_UNIT(m_UnitID);
	if (!unit) {
		return FUCK_ERR__CANNOT_FIND_UNIT;
	}

	if (unit->isIdle()) {

		int countSCV = AI_MODULE->GetUnitCount(BWAPI::UnitTypes::Terran_SCV);
		int countCommandCenter = AI_MODULE->GetUnitCount(BWAPI::UnitTypes::Terran_Command_Center);

		if (AI_MODULE->GetMoney() >= 50
			&& AI_MODULE->GetSupply() > 0
			&& countCommandCenter > 0
			&& countSCV / countCommandCenter < 15
			) {
			unit->train(BWAPI::UnitTypes::Terran_SCV);
		}
	}

	return FUCK_SUCCESS;
}


int UnitBrain_CommandCenter::OnDraw() {

	BWAPI::Unit unit = FIND_UNIT(m_UnitID);
	if (!unit) {
		return FUCK_ERR__NULL_UNIT;
	}

	std::map<int, TileEx>::iterator fuckIT = m_BuildableTilesMap.begin();
	for (fuckIT; fuckIT != m_BuildableTilesMap.end(); fuckIT++) {
		BWAPI::TilePosition tp = fuckIT->second.m_TilePosition;
		BWAPI::Broodwar->drawBoxMap(
			tp.x*TILE_SIZE, tp.y*TILE_SIZE,
			(tp.x + 1)*TILE_SIZE, (tp.y + 1) *TILE_SIZE,
			BWAPI::Colors::Grey);
		BWAPI::Broodwar->drawTextMap(tp.x*TILE_SIZE, tp.y*TILE_SIZE, "%u", fuckIT->second.GetKey());
	}

	std::list<int>::iterator mineralIT = m_Minerals.begin();
	for (mineralIT; mineralIT != m_Minerals.end(); mineralIT++) {
		BWAPI::Unit mineralUnit = FIND_UNIT(*mineralIT);
		if (mineralUnit) {
			BWAPI::Broodwar->drawCircleMap(mineralUnit->getPosition(), 20, BWAPI::Colors::Red);
		}
	}

	std::list<int>::iterator gasIT = m_GasStations.begin();
	for (gasIT; gasIT != m_GasStations.end(); gasIT++) {
		BWAPI::Unit gasUnit = FIND_UNIT(*gasIT);
		if (gasUnit) {
			BWAPI::Broodwar->drawCircleMap(gasUnit->getPosition(), 50, BWAPI::Colors::Red);
		}
	}

	BWAPI::Broodwar->drawCircleMap(unit->getPosition(), COMMAND_CENTER_RANGE, BWAPI::Colors::Yellow);

	return FUCK_SUCCESS;
}


//
//	递归警告, 扩张可建造图块集合
//	调用前请将recursiveCount归零
//
void UnitBrain_CommandCenter::ExtendBuildableTiles_Recursive(TileEx seed, BWAPI::Unit cc){
	
	std::list<TileEx> output = seed.GetBuildableNeighbor();
	std::list<TileEx>::iterator fuckIT = output.begin();

	for (fuckIT; fuckIT != output.end(); fuckIT++) {
		int key = (*fuckIT).GetKey();
		BWAPI::Position p = BWAPI::Position((*fuckIT).m_TilePosition.x*TILE_SIZE, (*fuckIT).m_TilePosition.y*TILE_SIZE);
		if (m_BuildableTilesMap.find(key) == m_BuildableTilesMap.end() && cc->getDistance(p) < COMMAND_CENTER_RANGE) {
			m_BuildableTilesMap.insert(make_pair(key, (*fuckIT)));
			ExtendBuildableTiles_Recursive(*fuckIT, cc);
		}
	}
}
