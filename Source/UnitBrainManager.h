#pragma once
#include <BWAPI.h>

class UnitBrain;

class UnitBrainManager {

public:

	std::map<int, UnitBrain*> m_UnitBrainMap;

	// �����������¼������UnitBrain,�������������Ҳ���Unit,UnitID����ȵ�.
	// ��Update��, ���list�е�Obj���ᱻ����
	std::list<int> m_DeadList;

	int OnInit();

	int OnUpdate();

	int CreateBrain(int unitID);

	UnitBrain* FindBrain(int unitID);
};