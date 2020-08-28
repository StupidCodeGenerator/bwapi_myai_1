#pragma once
#include <BWAPI.h>

class UnitBrain {

public:

	// 获得对应Unit的UnitID
	virtual int GetUnitID() = 0;

	// 初始化
	virtual int OnInit(int unitID) = 0;

	// 逻辑刷新
	virtual int OnUpdate() = 0;

};