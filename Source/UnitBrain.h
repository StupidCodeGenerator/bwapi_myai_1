#pragma once
#include <BWAPI.h>

class UnitBrain {

public:

	// ��ö�ӦUnit��UnitID
	virtual int GetUnitID() = 0;

	// ��ʼ��
	virtual int OnInit(int unitID) = 0;

	// �߼�ˢ��
	virtual int OnUpdate() = 0;

};