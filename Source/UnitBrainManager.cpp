#include "UnitBrainManager.h"
#include "UnitBrain.h"
#include "utils.h"
#include "MyAiModule.h"

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
		if (fuckIT->second->OnUpdate() < 0 || fuckIT->second->m_UnitID != fuckIT->first) {
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
//	不需要OnDestory, 因为在Update中会判定如果此单位找不到了,Update返回-1, Manager干死他即可
//	OnCreate事件触发的时候, getUnits并不会找到这个Unit,所以事实上会触发销毁.
//	解决的办法是,不用这个事件,它不靠谱. 
//	在Update中有一个遍历过程,遍历过程修改了Unit表, 然后根据Unit表更新UnitBrain表.
//	也就是, 如果一个Unit的ID,无对应的Brain,则创建Brain
//	然后Brain自己会判定Update中消失的Brain
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
