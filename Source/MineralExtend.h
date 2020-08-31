#pragma once

#include <BWAPI.h>
#include "utils.h"

//
//	水晶扩展逻辑, 包括是否被某个SCV盯上等. 
//
class MineralExtend {

private:
	
	int m_SCV_Count = 0;           // 表示当前有几个SCV盯上了这个矿(就是已经决定采了就算, 不管有没有开始采)
	
public:

	int m_UnitID = NULL_UNIT_ID;

	void OnInit(int unitID);
	
	void AddSCV();                 // 增加SCV计数

	void RemoveSCV();              // 减少SCV计数

	int GetSCVCount();
};
