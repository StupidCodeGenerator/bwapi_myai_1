#pragma once
#include <BWAPI.h>

class TileEx {

public:

	BWAPI::TilePosition m_TilePosition;

	void OnInit(BWAPI::TilePosition tile);

	int GetKey();
	
	bool IsBuildable();

	// 这个图块到某个单位的距离
	int DistanceToUnit(BWAPI::Unit unit);

	// 获得相邻的可建造图块
	// 这个可以用来递归扩充可建造集合
	std::list<TileEx> GetBuildableNeighbor();

};