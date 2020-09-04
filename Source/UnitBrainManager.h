#pragma once
#include <BWAPI.h>

class UnitBrain;

class UnitBrainManager {

public:

	std::map<int, UnitBrain*> m_UnitBrainMap;

	// ��ӪID��
	std::list<int> m_CommandCenters;

	// �����������¼������UnitBrain,�������������Ҳ���Unit,UnitID����ȵ�.
	// ��Update��, ���list�е�Obj���ᱻ����
	std::list<int> m_DeadList;

	int OnInit();

	int OnUpdate();

	int OnDraw();

	int OnUnitCreate(BWAPI::Unit unit);

	int CreateBrain(BWAPI::Unit unit);

	UnitBrain* FindBrain(int unitID);

	// ��Ӫˢ���ܱߵ�����Ϣ. 
	void RefreshCommmandCenterMapInfo();
};