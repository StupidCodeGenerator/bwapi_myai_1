#pragma once
#include "BWAPI.h"

using namespace BWAPI;
using namespace Filter;


#define    STATE_NULL       0
#define    STATE_DEFENCE    1
#define    STATE_ATTACK     2

#define    ATTACK_DELAY     200

class FrontManager {
public :

	// Set 1000 if under attack
	int underAttackTimeCount;

	int attackLimit;

	int lingCount;

	// Singleton
	static FrontManager* GetInstance() {
		if (!m_pInstance) {
			m_pInstance = new FrontManager();
		}
		return m_pInstance;
	}

	// Event
	void OnStart();
	void OnFrame();
	void OnUnitMorph(UnitInterface* pUnit);
	void OnUnitComplete(UnitInterface* pUnit);
	void OnUnitDestory(UnitInterface* pUnit);

	// States
	void SetState(int state);
	int GetState();
	char* GetStateStr();
	void StateInit_Defence();
	void StateOnFrame_Defence();
	void StateInit_Attack();
	void StateOnFrame_Attack();

private:


	FrontManager();
	~FrontManager();

	static FrontManager* m_pInstance;

	int m_READ_ONLY_State;

};