#pragma once
#include <BWAPI.h>
#include "UnitBrain.h"
#include "utils.h"
#include "TileEx.h"


class UnitBrain_CommandCenter : public UnitBrain {
public:

	int m_UnitID = NULL_UNIT_ID;

	std::map<int, TileEx> m_BuildableTilesMap;

	// 此基地范围内的矿
	std::list<int> m_Minerals;

	// 此基地范围内的Gas
	std::list<int> m_GasStations; 

	virtual int GetUnitID();

	virtual int OnInit(int unitID);

	void RefreshBuildableTiles();

	virtual int OnUpdate();

	virtual int OnDraw();
	
	// 扩展m_BuildableTiles, 一开始要放入一个图块作为种子
	// *递归调用警告*
	void ExtendBuildableTiles_Recursive(TileEx seed, BWAPI::Unit cc);

};