#include "MyAiModule.h"
#include "UnitBrain.h"
#include "utils.h"


int UnitBrain::OnInit(int unitID) {
	
	this->m_UnitID = unitID;
	return this->m_UnitID;
}


int UnitBrain::OnUpdate() {

	if (this->m_UnitID < 0) {
		return FUCK_SUCCESS;
	}

	BWAPI::Unit pUnit = AI_MODULE->FindUnit(this->m_UnitID);
	if (!pUnit) {
		return FUCK_ERR__CANNOT_FIND_UNIT;
	}

	return FUCK_SUCCESS;

}
