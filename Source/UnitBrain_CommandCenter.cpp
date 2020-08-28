#include "UnitBrain_CommandCenter.h"
#include "utils.h"
#include <BWAPI.h>
#include "MyAiModule.h"

int UnitBrain_CommandCenter::GetUnitID() {
	return this->m_UnitID;
}

int UnitBrain_CommandCenter::OnInit(int unitID) {
	this->m_UnitID = unitID;
	return this->m_UnitID;
}


//
//	OK, һ���߼�: 
//	���SCV����, CommandCenter����, �Լ��˿�����, ��Ǯ
//	�����Ǯ > 50, �˿�����δ��, SCV���� / CommandCenter���� < 15, ���Ҵ��ڿ���״̬, ����ũ��
//
int UnitBrain_CommandCenter::OnUpdate() {

	if (!AI_MODULE) {
		return FUCK_ERR__NULL_AI_MODULE;
	}

	BWAPI::Unit unit = AI_MODULE->FindUnit(m_UnitID);
	if (!unit) {
		return FUCK_ERR__CANNOT_FIND_UNIT;
	}

	if (unit->isIdle()) {

		int countSCV = AI_MODULE->GetUnitCount(BWAPI::UnitTypes::Terran_SCV);
		int countCommandCenter = AI_MODULE->GetUnitCount(BWAPI::UnitTypes::Terran_Command_Center);

		if (AI_MODULE->GetMoney() >= 50
			&& AI_MODULE->GetSupply() > 0
			&& countCommandCenter > 0
			&& countSCV / countCommandCenter < 15
			) {
			unit->train(BWAPI::UnitTypes::Terran_SCV);
		}
	}


	return FUCK_SUCCESS;
}