#pragma once
#include <BWAPI.h>

class UnitBrain;

class UnitBrainManager {

public:

	std::map<int, UnitBrain*> m_UnitBrainMap;

	// 这个是用来记录死掉的UnitBrain,包括但不限于找不到Unit,UnitID错误等等.
	// 在Update中, 这个list中的Obj都会被干死
	std::list<int> m_DeadList;

	int OnInit();

	int OnUpdate();

	int CreateBrain(int unitID);

	UnitBrain* FindBrain(int unitID);
};