#pragma once
#include <BWAPI.h>
//
//	资源管理器
//
//	用来辅助SCV采矿决策, 以及资源分配等. 包括需要预留多少钱, 需要开几片矿, 现在有多少矿, 都在什么地方, 
//	SCV可以查询这个资源管理器用来获得一个最优的矿进行采集等等. 
//

class MineralExtend;

class ResourceManager {

public:

	std::map<int, MineralExtend*> m_MineralsMap;

	int m_SystemTime;

	int OnInit();

	int OnUpdate();

	int CreateMineralExtend(BWAPI::Unit unit);

	// 获得一个SCV数量最少的矿.
	// 如果SCV数量都一致, 则找一个距离最近的
	// maxDistance 表示距离上限, 如果超过了这个距离, 则忽视这个矿
	// 因为SCV远程采矿是一个比较特殊的逻辑
	int FindMineralUnitID_of_MinSCV(int maxDistance, BWAPI::Unit scvUnit);

	std::list<int> FindMineralUnitInRange(BWAPI::Position center, int radius);
	std::list<int> FindGasUnitInRange(BWAPI::Position center, int radius);

	int AddSCV_of_Mineral(int mineralUnitID);       // 通知某块矿SCV霸占了
	int RemoveSCV_of_Mineral(int mineralUnitID);    // 通知某块矿SCV减少一个霸占

	void OnDraw();
};