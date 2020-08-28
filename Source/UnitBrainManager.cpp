#include "UnitBrainManager.h"
#include "UnitBrain.h"
#include "utils.h"
#include "MyAiModule.h"

int UnitBrainManager::OnInit() {

	// �����ʼ����ʱ��,Unit��Ϊ��,����Ҫ����һ��,�ͷ����е��ڴ�Ȼ������ձ�
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
	
	// Update,��������������Brain
	std::map<int, UnitBrain*>::iterator fuckIT = m_UnitBrainMap.begin();
	for (fuckIT; fuckIT != m_UnitBrainMap.end(); fuckIT++) {
		if (fuckIT->second->OnUpdate() < 0 || fuckIT->second->m_UnitID != fuckIT->first) {
			m_DeadList.push_back(fuckIT->first);
		}
	}

	// ɾ��������Brain
	std::list<int>::iterator deadIT = m_DeadList.begin();
	for (deadIT; deadIT != m_DeadList.end(); deadIT++) {
		m_UnitBrainMap.erase(*deadIT);
	}

	return FUCK_SUCCESS;

}


//
//	����ҪOnDestory, ��Ϊ��Update�л��ж�����˵�λ�Ҳ�����,Update����-1, Manager����������
//	OnCreate�¼�������ʱ��, getUnits�������ҵ����Unit,������ʵ�ϻᴥ������.
//	����İ취��,��������¼�,��������. 
//	��Update����һ����������,���������޸���Unit��, Ȼ�����Unit�����UnitBrain��.
//	Ҳ����, ���һ��Unit��ID,�޶�Ӧ��Brain,�򴴽�Brain
//	Ȼ��Brain�Լ����ж�Update����ʧ��Brain
//
int UnitBrainManager::CreateBrain(int unitID) {

	UnitBrain* pFuckBrain = new UnitBrain();

	int errOfInit = pFuckBrain->OnInit(unitID);
	if (errOfInit < 0) {
		SAFE_DELETE(pFuckBrain);
		return errOfInit;
	}

	m_UnitBrainMap.insert(make_pair(unitID, pFuckBrain));
	
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
