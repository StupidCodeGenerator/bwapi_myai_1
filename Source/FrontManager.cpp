#include "FrontManager.h"
#include "BackManager.h"
#include "InformationManager.h"

FrontManager* FrontManager::m_pInstance;

FrontManager::FrontManager() {}
FrontManager::~FrontManager() {}


//____EVENTS___________________________________________________________________


void FrontManager::OnStart() {
	underAttackTimeCount = 0;
	attackLimit = 40;

	SetState(STATE_DEFENCE);
}


/*
	Not like the StateOnFrame functions.
	This function is always running, in all m_READ_ONLY_State.
	Some kind of action is stateless, like defence.
*/
void FrontManager::OnFrame() {

	// Once some of my units are under attack, start defence.
	// if it's zero, stop.
	if (underAttackTimeCount > 0) {
		underAttackTimeCount--;
	}

	// Count the lings. And under attack check
	// If more types of froces are included in further development,
	// will use force's supplies instead
	lingCount = 0;
	for (auto &u : Broodwar->self()->getUnits()) {
		if (u->getType() == UnitTypes::Zerg_Zergling) {
			lingCount++;
		}
		if (u->isUnderAttack()) {
			underAttackTimeCount = ATTACK_DELAY;
		}
	}

	// States
	switch (this->GetState()) {
		default:
		case STATE_NULL:
			break;
		case STATE_DEFENCE:
			StateOnFrame_Defence();
			break;
		case STATE_ATTACK:
			StateOnFrame_Attack();
			break;
	}
}


void FrontManager::OnUnitMorph(UnitInterface* pUnit) {
}

void FrontManager::OnUnitComplete(UnitInterface* pUnit) {
}

void FrontManager::OnUnitDestory(UnitInterface* pUnit) {
	underAttackTimeCount = ATTACK_DELAY;
}


//____STATES___________________________________________________________________


void FrontManager::SetState(int state) {
	switch (state) {
		default:
		case STATE_NULL:
			return;                 // RETURN HERE !! (Not in case, so invalid)
		case STATE_DEFENCE:
			StateInit_Defence();
			break;
		case STATE_ATTACK:
			StateInit_Attack();
			break;
	}

	m_READ_ONLY_State = state;
}


int FrontManager::GetState() {
	return m_READ_ONLY_State;
}


char* FrontManager::GetStateStr() {
	switch (m_READ_ONLY_State) {
		default:
		case STATE_NULL:
			return "NULL";
		case STATE_DEFENCE:
			return "DEFENCE";
		case STATE_ATTACK:
			return "ATTACK";
	}
}


void FrontManager::StateInit_Defence() {

}


void FrontManager::StateOnFrame_Defence() {
	
	if ((lingCount >= attackLimit && InformationManager::GetInstance()->pEnemyTilePosition)) { // Attack !
		BackManager::GetInstance()->AddPlan(UnitTypes::Zerg_Drone, 3);
		SetState(STATE_ATTACK);
	}

	// DEFENCE
	bool isUnderAttack = false;
	for (auto &u : Broodwar->self()->getUnits()) {
		if (u->getType() == UnitTypes::Zerg_Zergling) {
			if (u->getClosestUnit(Filter::IsEnemy)) {
				isUnderAttack = true;
				u->attack(u->getClosestUnit(Filter::IsEnemy));
			}
		}
	}
	if (!isUnderAttack) {
		for (auto &u : Broodwar->self()->getUnits()) {
			if (u->getType() == UnitTypes::Zerg_Zergling) {
				u->move((Position)Broodwar->self()->getStartLocation());
			}
		}
	}
}

/*
	If the first overlord didn't find the enemy point, then send an Drone to search
*/
void FrontManager::StateInit_Attack() {

}


void FrontManager::StateOnFrame_Attack() {

	if (!Broodwar->enemy()->getUnits().empty()) {
		for (auto &u : Broodwar->enemy()->getUnits()) {
			if (u->getPosition().getDistance((Position)Broodwar->self()->getStartLocation()) < 200 && !u->getType().isWorker()) {
				SetState(STATE_DEFENCE);
				break;
			}
		}
	}

	if (lingCount < (double)attackLimit * 0.6) {
		SetState(STATE_DEFENCE);
		attackLimit += 15;
	}

	if (lingCount >= 1) {
		for (auto &u : Broodwar->self()->getUnits()) {
			if (u->getType() == UnitTypes::Zerg_Zergling) {
				if (underAttackTimeCount > 0 && u->getClosestUnit(Filter::IsEnemy)) {
					u->attack(u->getClosestUnit(Filter::IsEnemy));
				} else if (InformationManager::GetInstance()->pEnemyTilePosition) {
					u->attack((Position)(*InformationManager::GetInstance()->pEnemyTilePosition));
				}
			}
		}
	}

}


