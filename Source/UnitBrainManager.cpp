#include "UnitBrainManager.h"
#include "UnitBrain.h"
#include "utils.h"
#include "MyAiModule.h"
#include "UnitBrain_CommandCenter.h"

int UnitBrainManager::OnInit() {

	// 如果初始化的时候,Unit表不为空,则需要遍历一遍,释放所有的内存然后再清空表
	if (!m_UnitBrainMap.empty()) {
		std::map<int, UnitBrain*>::iterator fuckIT = m_UnitBrainMap.begin();
		for (fuckIT; fuckIT != m_UnitBrainMap.end(); fuckIT++) {
			SAFE_DELETE(fuckIT->second);
		}
	}
	m_UnitBrainMap.clear();

	return FUCK_SUCCESS;

}


int UnitBrainManager::OnUpdate() {

	m_DeadList.clear();

	// Update,并且甄别出死掉的Brain
	std::map<int, UnitBrain*>::iterator fuckIT = m_UnitBrainMap.begin();
	for (fuckIT; fuckIT != m_UnitBrainMap.end(); fuckIT++) {
		if (fuckIT->second->OnUpdate() < 0 || fuckIT->second->GetUnitID() != fuckIT->first) {
			m_DeadList.push_back(fuckIT->first);
		}
	}

	// 删掉死掉的Brain
	std::list<int>::iterator deadIT = m_DeadList.begin();
	for (deadIT; deadIT != m_DeadList.end(); deadIT++) {
		m_UnitBrainMap.erase(*deadIT);
	}

	return FUCK_SUCCESS;

}


//
//	创建一个UnitBrain, 根据类型进行区分, 创建的是UnitBrain的子类
//	另外, 销毁在Update中, 不是事件中
//
int UnitBrainManager::CreateBrain(BWAPI::Unit unit) {

	if (!unit) {
		return FUCK_ERR__NULL_UNIT;
	}

	UnitBrain* pFuckBrain = NULL;

	// 根据
	switch (unit->getType()) {
	default:
		pFuckBrain = NULL;

	case BWAPI::UnitTypes::Terran_Command_Center:
		pFuckBrain = new UnitBrain_CommandCenter();
		break;
	case BWAPI::UnitTypes::Terran_SCV:
		// TODO: Create a SCV UnitBrain
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
//	找到指定的 Brain
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
