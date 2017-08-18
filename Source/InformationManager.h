#pragma once
#include "BWAPI.h"
#include "ResourcePoint.h"

using namespace BWAPI;


#define    STATE_NULL             0
#define    STATE_FIRST_SEARCH     1
#define    STATE_IDLE             2

class InformationManager {

public:

	// Enemy Point, NULL if not found
	TilePosition* pEnemyTilePosition;

	// 3 possible enemyPoints, FirstPoint should be the closest.
	TilePosition* pFirstSearchPoint;
	TilePosition* pSecondSearchPoint;
	TilePosition* pThirdSearchPoint;

	// Searchers
	UnitInterface* pFirstSearchingOverlord;
	UnitInterface* pFirstSearchingDrone;

	// Singleton
	static InformationManager* GetInstance() {
		if (!m_pInstance) {
			m_pInstance = new InformationManager();
		}
		return m_pInstance;
	}

	// Events
	void OnStart();
	void OnFrame();
	void OnUnitMorph(Unit unit);
	void OnUnitDestory(Unit unit);
	void OnUnitComplete(Unit unit);
	void CountUnitOfType(UnitType type);

	// States
	void SetState(int state);
	int GetState();
	char* GetStateStr();
	void StateInit_FirstSearch();
	void StateOnFrame_FirstSearch();

	// Others
	void FindSearchPositions();

private:

	InformationManager();
	~InformationManager();

	static InformationManager* m_pInstance;

	int m_READ_ONLY_State;

};