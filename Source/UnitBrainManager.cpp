#include "UnitBrainManager.h"
#include "UnitBrain.h"
#include "utils.h"
#include "MyAiModule.h"
#include "UnitBrain_CommandCenter.h"
#include "UnitBrain_SCV.h"

int UnitBrainManager::OnInit() {

	// �����ʼ����ʱ��,Unit��Ϊ��,����Ҫ����һ��,�ͷ����е��ڴ�Ȼ������ձ�
	if (!m_UnitBrainMap.empty()) {
		std::map<int, UnitBrain*>::iterator fuckIT = m_UnitBrainMap.begin();
		for (fuckIT; fuckIT != m_UnitBrainMap.end(); fuckIT++) {
			SAFE_DELETE(fuckIT->second);
		}
	}
	m_UnitBrainMap.clear();
	m_CommandCenters.clear();

	return FUCK_SUCCESS;

}


int UnitBrainManager::OnUpdate() {

	m_DeadList.clear();

	// Update,��������������Brain
	std::map<int, UnitBrain*>::iterator fuckIT = m_UnitBrainMap.begin();
	for (fuckIT; fuckIT != m_UnitBrainMap.end(); fuckIT++) {
		if (fuckIT->second->OnUpdate() < 0 || fuckIT->second->GetUnitID() != fuckIT->first) {
			m_DeadList.push_back(fuckIT->first);
			SAFE_DELETE(fuckIT->second);  // ˳���second�ͷ���
		}
	}

	// ɾ��������Brain
	std::list<int>::iterator deadIT = m_DeadList.begin();
	for (deadIT; deadIT != m_DeadList.end(); deadIT++) {
		m_UnitBrainMap.erase(*deadIT);
	}

	return FUCK_SUCCESS;

}


int UnitBrainManager::OnDraw() {
	std::map<int, UnitBrain*>::iterator fuckIT = m_UnitBrainMap.begin();
	for (fuckIT; fuckIT != m_UnitBrainMap.end(); fuckIT++) {
		if (fuckIT->second) {
			fuckIT->second->OnDraw();
		}
	}

	return FUCK_SUCCESS;
}


int UnitBrainManager::OnUnitCreate(BWAPI::Unit unit) {
	if (unit && unit->getPlayer()->getID() == SELF->getID()) {
		CreateBrain(unit);
	}

	RefreshCommmandCenterMapInfo();

	return FUCK_SUCCESS;
}


//
//	����һ��UnitBrain, �������ͽ�������, ��������UnitBrain������
//	����, ������Update��, �����¼���
//
int UnitBrainManager::CreateBrain(BWAPI::Unit unit) {

	if (!unit || !unit->exists()) {
		return FUCK_ERR__NULL_UNIT;
	}

	if (m_UnitBrainMap.find(unit->getID()) != m_UnitBrainMap.end()) {
		return FUCK_ERR__UNIT_ALREADY_IN;
	}

	UnitBrain* pFuckBrain = NULL;

	// ����
	switch (unit->getType()) {
	default:
		pFuckBrain = NULL;
		break;
	case BWAPI::UnitTypes::Terran_Command_Center:
		pFuckBrain = new UnitBrain_CommandCenter();
		m_CommandCenters.push_back(unit->getID());
		break;
	case BWAPI::UnitTypes::Terran_SCV:
		pFuckBrain = new UnitBrain_SCV();
		break;
	}

	if (!pFuckBrain) {
		return FUCK_ERR__UNKNOWN_UNIT_TYPE;
	}

	int errOfInit = pFuckBrain->OnInit(unit->getID());
	if (errOfInit < 0) {
		SAFE_DELETE(pFuckBrain);
		return errOfInit;
	}

	m_UnitBrainMap.insert(make_pair(unit->getID(), pFuckBrain));

	return FUCK_SUCCESS;
}


//
//	�ҵ�ָ���� Brain
//
UnitBrain* UnitBrainManager::FindBrain(int unitID) {
	std::map<int, UnitBrain*>::iterator fuckIT = m_UnitBrainMap.find(unitID);
	if (fuckIT == m_UnitBrainMap.end()) {
		return NULL;
	}
	else {
		return fuckIT->second;
	}
}


void UnitBrainManager::RefreshCommmandCenterMapInfo() {
	std::list<int>::iterator fuckIT = m_CommandCenters.begin();
	for (fuckIT; fuckIT != m_CommandCenters.end(); fuckIT++) {
		UnitBrain_CommandCenter* pBrain = (UnitBrain_CommandCenter*)(FindBrain(*fuckIT));
		pBrain->RefreshBuildableTiles();
	}
}
