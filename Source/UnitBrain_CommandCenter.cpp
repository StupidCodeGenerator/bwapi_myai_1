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

	m_OrderedTilesMap.clear();

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

		tileIT->second.m_DistanceToCommandCenter = tileIT->second.DistanceToUnit(unit);
		int key = tileIT->second.GetKey();

		if (m_OrderedTilesMap.find(key) != m_OrderedTilesMap.end()) {
			toRemove.push_back(tileIT->first);
		}
		else if (tileIT->second.m_DistanceToCommandCenter < COMMAND_CENTER_MIN_BUILD_DISTANCE) {
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
	// TODO : 目前来说, 仅仅是"不要在离矿太近的地方以及离主基地太近的地方建造"就已经实现了这个功能
	// 但是,对于那种矿离得比较远的地形, 可能会出现问题. 
	// 目前就先这样

	std::list<int>::iterator removeIT = toRemove.begin();
	for (removeIT; removeIT != toRemove.end(); removeIT++) {
		m_BuildableTilesMap.erase(*removeIT);
	}

	// 最后, 放到m_BuildableTiles_SortByDistance 里面, 进行排序
	// 前面在进行过滤的时候, 已经设置了 DistanceToCommandCenter
	m_SortedBuildableTilesList.clear();
	std::map<int, TileEx>::iterator finalIT = m_BuildableTilesMap.begin();
	for (finalIT; finalIT != m_BuildableTilesMap.end(); finalIT++) {
		m_SortedBuildableTilesList.push_back(finalIT->second);
	}
	m_SortedBuildableTilesList.sort();
}



// 按照从近到远的顺序依次给出就好, 当SCV开始建造的时候, 会刷新Buildable
// 返回的要给一个"占用"标
// 由于BuildableTiles 可能会被刷新, 所以需要一个单独的map来存那些被占用的位置
// 就是说, 已经被取走预定, 虽然可能还没有建造, 但是已经有可能要建造了
// 如果发生了建造图块用光的情况, 那就是二阶逻辑了.现在先不管
BWAPI::TilePosition UnitBrain_CommandCenter::GetBuildPosition(int tileWidth, int tileHeight) {
	std::list<TileEx>::iterator fuckIT = m_SortedBuildableTilesList.begin();
	for (fuckIT; fuckIT != m_SortedBuildableTilesList.end(); fuckIT++) {
		std::list<int> buildableTiles = GetTileKeysIfAreaBuildable(*fuckIT, tileWidth, tileHeight);
		if (buildableTiles.empty()) {
			continue;
		}
		else {
			std::list<int>::iterator tileIT = buildableTiles.begin();
			for (tileIT; tileIT != buildableTiles.end(); tileIT++) {
				int tileKey = *tileIT;
				std::map<int, TileEx>::iterator mapIT = m_BuildableTilesMap.find(tileKey);
				if (mapIT != m_BuildableTilesMap.end()) {
					m_OrderedTilesMap.insert(make_pair(mapIT->first, mapIT->second));
				}
			}
			return (*fuckIT).m_TilePosition;
		}
	}

	// *这里进入二阶逻辑
	// 暂时先这么写
	return BWAPI::TilePosition(0, 0);  // that means empty, and i never use the 0,0 position
}

// 区域可建造
bool UnitBrain_CommandCenter::IsAreaBuildable(TileEx tile, int tileWidth, int tileHeight) {
	return GetTileKeysIfAreaBuildable(tile, tileWidth, tileHeight).size() > 0;
}


//	图块是可建造
bool UnitBrain_CommandCenter::IsTileBuildable(int tileKey) {
	return m_BuildableTilesMap.find(tileKey) == m_BuildableTilesMap.end() || m_OrderedTilesMap.find(tileKey) != m_BuildableTilesMap.end();
}

//
//	如果没有找到返回emptylist
//
std::list<int> UnitBrain_CommandCenter::GetTileKeysIfAreaBuildable(TileEx tile, int tileWidth, int tileHeight) {
	std::list<int> output;
	int initX = tile.m_TilePosition.x;
	int initY = tile.m_TilePosition.y;
	for (int x = initX; x < initX + tileWidth; x++) {
		for (int y = initY; y < initY + tileHeight; y++) {
			TileEx temp;
			temp.OnInit(BWAPI::TilePosition(x, y));
			int key = temp.GetKey();
			if (IsTileBuildable(key)) {
				// 不可建造
				output.clear();
				return output;
			}
			else {
				output.push_back(key);
			}
		}
	}

	return output;
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

	std::list<TileEx>::iterator fuckIT = m_SortedBuildableTilesList.begin();
	for (fuckIT; fuckIT != m_SortedBuildableTilesList.end(); fuckIT++) {
		BWAPI::TilePosition tp = (*fuckIT).m_TilePosition;
		BWAPI::Broodwar->drawDotMap(tp.x*TILE_SIZE, tp.y*TILE_SIZE, BWAPI::Colors::Grey);
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
void UnitBrain_CommandCenter::ExtendBuildableTiles_Recursive(TileEx seed, BWAPI::Unit cc) {

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
