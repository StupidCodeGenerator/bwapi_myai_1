#include "UnitBrain_SCV.h"
#include <BWAPI.h>
#include "MyAiModule.h"
#include "ResourceManager.h"

int UnitBrain_SCV::GetUnitID() {
	return m_UnitID;
}


int UnitBrain_SCV::OnInit(int unitID) {
	m_UnitID = unitID;
	return FUCK_SUCCESS;
}


int UnitBrain_SCV::OnUpdate() {
	if (!AI_MODULE) {
		return FUCK_ERR__NULL_AI_MODULE;
	}

	BWAPI::Unit unit = FIND_UNIT(m_UnitID);
	if (!unit) {
		return FUCK_ERR__CANNOT_FIND_UNIT;
	}

	if (!AI_MODULE->m_pResourceManager) {
		return FUCK_ERR__NULL_RESOURCE_MANAGER;
	}

	// 一阶逻辑: 不断采矿,手里有矿就回家,然后继续采

	if (unit->isIdle()) {
		if (unit->isCarryingMinerals() || unit->isCarryingGas()) {
			unit->returnCargo();
		}
		else {
			int mineralUnitID = AI_MODULE->m_pResourceManager->FindMineralUnitID_of_MinSCV(SCV_MINERAL_MAX_DISTRANCE, unit);
			BWAPI::Unit mineralUnit = AI_MODULE->FindMinerals(mineralUnitID);
			if (mineralUnit) {
				unit->gather(mineralUnit);
				AI_MODULE->m_pResourceManager->AddSCV_of_Mineral(mineralUnitID);
			}
		}
	}

	return FUCK_SUCCESS;
}


int UnitBrain_SCV::OnDraw() {
	return FUCK_SUCCESS;
}
