#pragma once
#include <BWAPI.h>
#include "UnitBrain.h"
#include "utils.h"
#include "TileEx.h"


class UnitBrain_CommandCenter : public UnitBrain {
public:

	int m_UnitID = NULL_UNIT_ID;

	std::map<int, TileEx> m_BuildableTilesMap;            // 键值对方式存储的图块表, 用来生成建造点等一些需要键值关系的地方
	std::list<TileEx> m_SortedBuildableTilesList;         // 排序后的图块表, 用来快速获得建造点

	std::map<int, TileEx> m_OrderedTilesMap;              // 被预定的图块, 一般情况下是某个SCV请求了图块要拿去建造了

	// 此基地范围内的矿
	std::list<int> m_Minerals;

	// 此基地范围内的Gas
	std::list<int> m_GasStations; 

	virtual int GetUnitID();

	virtual int OnInit(int unitID);

	void RefreshBuildableTiles();

	// 像Center获取一个可以建造的位置
	BWAPI::TilePosition GetBuildPosition(int tileWidth, int tileHeight);

	bool IsAreaBuildable(TileEx tile, int tileWidth, int tileHeight);
	bool IsTileBuildable(int tileKey);
	std::list<int> GetTileKeysIfAreaBuildable(TileEx tile, int tileWidth, int tileHeight);

	virtual int OnUpdate();

	virtual int OnDraw();
	
	// 扩展m_BuildableTiles, 一开始要放入一个图块作为种子
	// *递归调用警告*
	void ExtendBuildableTiles_Recursive(TileEx seed, BWAPI::Unit cc);

};