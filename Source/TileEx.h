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

	// CommandCenter专用的排序功能, 需要先存储了到CommandCenter的距离, 然后才能排序
	// 否则调用后, 结果无效
	
	int m_DistanceToCommandCenter = 0xFFFF;                  // 到CommandCenter的距离
	
	// 排序使用的操作符重载
	bool operator < (TileEx other);
	bool operator > (TileEx other);

};