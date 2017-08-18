#include "InformationManager.h"

using namespace BWAPI;

InformationManager* InformationManager::m_pInstance;


//____EVENTS___________________________________________________________________

void InformationManager::OnStart() {
	
	pEnemyTilePosition = NULL;
	pFirstSearchPoint = NULL;
	pSecondSearchPoint = NULL;
	pThirdSearchPoint = NULL;
	pFirstSearchingDrone = NULL;
	pFirstSearchingOverlord = NULL;

	// The first searching overlord
	FindSearchPositions();

	SetState(STATE_FIRST_SEARCH);
}


void InformationManager::OnFrame() {

	// Find out possible enemy points
	if (!pEnemyTilePosition) {
		for (auto &u : Broodwar->enemy()->getUnits()) {
			// Only buildings can be enemie's home.
			if (!u->getType().isBuilding()) {
				continue;
			}
			// If Buildings are found, find the closest start tilePosition as the 
			// Enemy's home point.
			int minDistance = -1;
			TilePosition* pMinPosition = NULL;
			for (auto &p : Broodwar->getStartLocations()) {
				int distance = p.getDistance(u->getTilePosition());
				if (distance < minDistance || minDistance < 0) {
					minDistance = distance;
					pMinPosition = &(TilePosition)p;
				}
			}
			if (!pMinPosition) {
				break;
			}
			pEnemyTilePosition = new TilePosition(pMinPosition->x, pMinPosition->y);
		}
	}

	// States
	switch (m_READ_ONLY_State) {
		default:
		case STATE_NULL:
			break;
		case STATE_FIRST_SEARCH:
			StateOnFrame_FirstSearch();
			break;
	}
}


void InformationManager::OnUnitMorph(Unit unit) {}
void InformationManager::OnUnitDestory(Unit unit) {}
void InformationManager::OnUnitComplete(Unit unit) {}
void InformationManager::CountUnitOfType(UnitType type) {}


//____STATES___________________________________________________________________

void InformationManager::SetState(int state) {
	switch (state) {
		default:
		case STATE_NULL:
			return;                 // RETURN HERE !! (Not in case, so invalid)
		case STATE_FIRST_SEARCH:
			StateInit_FirstSearch();
			break;
	}

	m_READ_ONLY_State = state;
}


int InformationManager::GetState() {
	return m_READ_ONLY_State;
}

char* InformationManager::GetStateStr() {
	switch (m_READ_ONLY_State) {
		default:
		case STATE_NULL:
			return "NULL";
		case STATE_FIRST_SEARCH:
			return "FIRST_SEARCH";
		case STATE_IDLE:
			return "IDLE";
	}
}

void InformationManager::StateInit_FirstSearch() {
	if (pFirstSearchPoint && !pFirstSearchingOverlord) {
		for (auto &u : Broodwar->self()->getUnits()) {
			if (u->getType() == UnitTypes::Zerg_Overlord) {
				u->move((Position)(*pFirstSearchPoint));
				u->move((Position)(Broodwar->self()->getStartLocation()), true);
				Broodwar->sendText("First Searcher is Start");
				pFirstSearchingOverlord = u;
			}
		}
	}
}
void InformationManager::StateOnFrame_FirstSearch() {
	
	// The Second Searcher
	if (!pEnemyTilePosition && !pFirstSearchingDrone && Broodwar->self()->supplyUsed() >= 18) {
		for (auto &u : Broodwar->self()->getUnits()) {
			if (u->getType() == UnitTypes::Zerg_Drone) {
				if (!u->isConstructing() && !u->isCarryingGas() && !u->isCarryingMinerals()) {
					// Goto the second point
					pFirstSearchingDrone = u;
					u->move((Position)(*pSecondSearchPoint));
					u->move((Position)(*pThirdSearchPoint), true);
					u->move((Position)(Broodwar->self()->getStartLocation()), true);
					Broodwar->sendText("Drone Searcher start");
					break;
				}
			}
		}
	}
}


//____OTHERS___________________________________________________________________


/*
	Find the 3 Enemy Points to search.
	First point must be the closest for the Overlord to search.
*/
void InformationManager::FindSearchPositions() {

	TilePosition myPosition = Broodwar->self()->getStartLocation();
	TilePosition::list startLocations = Broodwar->getStartLocations();

	// Find the closest
	TilePosition::list::iterator itor = startLocations.begin();
	double minDistance = -1;
	while (itor != startLocations.end()) {
		int x = itor->x;
		int y = itor->y;
		int myX = myPosition.x;
		int myY = myPosition.y;

		if (!(x == myX && y == myY)) {
			double distance = itor->getDistance(myPosition);
			if (minDistance < 0 || distance < minDistance) {
				minDistance = distance;
				if (pFirstSearchPoint != NULL) {
					free(pFirstSearchPoint);  // Relase pre defined 
				}
				pFirstSearchPoint = new TilePosition(x, y);
			}
		}
		itor++;
	}

	// After the closest point was found , find others.
	itor = startLocations.begin();
	while (itor != startLocations.end() && pFirstSearchPoint != NULL) {
		int x = itor->x;
		int y = itor->y;
		int myX = myPosition.x;
		int myY = myPosition.y;
		int firstX = pFirstSearchPoint->x;
		int firstY = pFirstSearchPoint->y;

		if (!(x == myX && y == myY) && !(x == firstX && y == firstY)) {
			if (pSecondSearchPoint == NULL) {
				pSecondSearchPoint = new TilePosition(x, y);
			} else if (pThirdSearchPoint == NULL) {
				pThirdSearchPoint = new TilePosition(x, y);
			}
		}
		itor++;
	}

	Broodwar->sendText("ClosestPoint(%d, %d)", pFirstSearchPoint->x, pFirstSearchPoint->y);

}

//_____________________________________________________________________________


InformationManager::InformationManager() {}
InformationManager::~InformationManager() {}

