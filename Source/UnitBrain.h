#pragma once
#include <BWAPI.h>

class UnitBrain {

public:

	int m_UnitID = 0;

	int OnInit(int unitID);

	int OnUpdate();

};