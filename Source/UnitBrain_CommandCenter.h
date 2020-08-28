#pragma once
#include <BWAPI.h>
#include "UnitBrain.h"

class UnitBrain_CommandCenter : public UnitBrain {

public:

	int m_UnitID = 0;

	virtual int GetUnitID();

	virtual int OnInit(int unitID);

	virtual int OnUpdate();
};