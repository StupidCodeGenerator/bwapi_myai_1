#pragma once
#include <BWAPI.h>
#include "utils.h"
#include "UnitBrain.h"


//
//	SCV �߼�
//	1���߼�: ʹ���ɿ󼴿�
//
class UnitBrain_SCV : public UnitBrain {

	int m_UnitID = NULL_UNIT_ID;

	virtual int GetUnitID();

	virtual int OnInit(int unitID);

	virtual int OnUpdate();

	virtual int OnDraw();

};