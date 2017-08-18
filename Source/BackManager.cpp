#include "BackManager.h"
#include "MyAiModule.h"
#include "FrontManager.h"

BackManager* BackManager::m_pInstance;


BackManager::BackManager() {
}


BackManager::~BackManager() {}


//____THE_PLAN_SYSTEM__________________________________________________________

/*
	In this function, will create a new tilePosition object.
	So beware of the leak.
	free your pTilePosition outside this function, after call.
*/
void BackManager::AddPlan(UnitType type, int num, TilePosition* pTilePosition) {
	Broodwar->sendText("AddPlan: %s, %d", type.c_str(), num);
	for (int i = 0; i < num; i++) {
		PlanNode* pNode;
		pNode = (PlanNode*)malloc(sizeof(PlanNode));
		pNode->type = type;

		if (pTilePosition) {
			pNode->pTilePosition = new TilePosition(pTilePosition->x, pTilePosition->y);
		} else {
			pNode->pTilePosition = NULL;
		}

		this->m_PlanList.push_back(pNode);
	}
}

void BackManager::AddPlan(UnitType type, int num) {

	this->AddPlan(type, num, NULL);
}

void BackManager::AddPlan(UnitType type) {
	this->AddPlan(type, 1, NULL);
}

void BackManager::AddPlan_Front(UnitType type, int num, TilePosition* pTilePosition) {
	Broodwar->sendText("AddPlan: %s, %d", type.c_str(), num);
	for (int i = 0; i < num; i++) {
		PlanNode* pNode;
		pNode = (PlanNode*)malloc(sizeof(PlanNode));
		pNode->type = type;

		if (pTilePosition) {
			pNode->pTilePosition = new TilePosition(pTilePosition->x, pTilePosition->y);
		} else {
			pNode->pTilePosition = NULL;
		}

		this->m_PlanList.push_front(pNode);
	}
}

void BackManager::AddPlan_Front(UnitType type, int num) {
	this->AddPlan_Front(type, num, NULL);
}

void BackManager::AddPlan_Front(UnitType type) {
	this->AddPlan_Front(type, 1, NULL);
}


//____EVENTS___________________________________________________________________

void BackManager::OnStart() {

	m_SupplyTotal = 0;

	m_PlanList.clear();
	m_Progress.clear();

	// Find all resource points
	for (auto &u : Broodwar->getAllUnits()) {
		if (u->getType().isMineralField()) {
			bool newPoint = true;
			for (auto &rp : m_ResourcePoints) {
				if (rp.AddThisPointIfInRange(u)) {     // If not in range , create new 
					newPoint = false;
				}
			}
			if (newPoint) {
				m_ResourcePoints.push_back(*(new ResourcePoint(u->getTilePosition())));
			}
		}
	}

	this->SetState(STATE_OPEN);
}


void BackManager::OnFrame() {

	// Unit Loop
	for (auto &u : Broodwar->self()->getUnits()) {
		// Let the work goto work
		if (u->getType().isWorker()) {
			WorkerGotoMinerals(u);
		}
	}

	// Check My m_ResourcePoints EVERY 100 FRAMES
	if (frameCount % 100 == 0) {
		for (auto &rp : m_ResourcePoints) {
			rp.tileDistanceFromMe = rp.tilePosition.getDistance(Broodwar->self()->getStartLocation());
			if (!rp.isMine) {
				for (auto &u : Broodwar->self()->getUnits()) {
					if (u->getType().isBuilding() && u->getTilePosition().getDistance(rp.tilePosition) < 30) {
						rp.isMine = true;
						break; // break to HERE
					}
				} // HERE  <--
			}
		}
	}

	// Update the plan
	this->UpdatePlan();

	// STATES
	switch (this->GetState()) {
		default:
		case STATE_NULL:
			break;
		case STATE_OPEN:
			StateOnFrame_Open();
			break;
		case STATE_LOTS_OF_LINGS:
			StateOnFrame_LotsOfLings();
			break;
	}


	frameCount++;
}


/*
	Iterate works in plan and do them
*/
void BackManager::UpdatePlan() {

	// Stop doing the work when the worker is building sth.
	// The Zerg's builder will die, so there's no "Constructing".
	// Only going to construct.
	for (auto &u : Broodwar->self()->getUnits()) {
		if (u->getType().isWorker() && u->isConstructing()) {
			return;
		}
	}

	// Do the work
	if (!m_PlanList.empty()) {
		// When some worker is going to build sth, stop the plan to save resource for the build.
		for (auto &u : Broodwar->self()->getUnits()) {
			if (u->getType().isWorker() && u->isConstructing()) {
				return;
			}
		}

		PlanNode* pCurrentNode = m_PlanList.front();

		if (pCurrentNode->type.isBuilding()) {

			for (auto &u : Broodwar->self()->getUnits()) {
				if (u->getType().isWorker() && u->getTilePosition().getDistance(Broodwar->self()->getStartLocation()) < 30) {
					if (pCurrentNode->pTilePosition == NULL) {
						if (
							u->build(pCurrentNode->type,
								Broodwar->getBuildLocation(pCurrentNode->type, u->getTilePosition()))
						) {
							m_Progress.push_back(pCurrentNode);
							m_PlanList.pop_front();
							break;
						}
					} else {
						if (u->build(pCurrentNode->type, *(pCurrentNode->pTilePosition))) {
							Broodwar->sendText("Build by position (%d, %d)", pCurrentNode->pTilePosition->x, pCurrentNode->pTilePosition->y);
							m_Progress.push_back(pCurrentNode);
							m_PlanList.pop_front();
							break;
						} else {
							Broodwar->sendText("Error %s", Broodwar->getLastError().c_str());
						}
					}
				}
			}

		} else {
			switch (pCurrentNode->type) {
				default:
					break;
				case UnitTypes::Zerg_Overlord:
				case UnitTypes::Zerg_Drone:
				case UnitTypes::Zerg_Zergling:
					for (auto &u : Broodwar->self()->getUnits()) {
						if (u->getType() == UnitTypes::Zerg_Larva) {
							if (u->train(pCurrentNode->type)) {
								m_Progress.push_back(pCurrentNode);
								m_PlanList.pop_front();
								break; // break the loop
							}
						}
					}
					break;
			}
		}
	}
}


/*
	For a zerg unit, the morph means it come out from an egg
*/
void BackManager::OnUnitMorph(Unit unit) {
}


void BackManager::OnUnitDestory(Unit unit) {
	if (unit->getType() == UnitTypes::Zerg_Overlord) {
		m_SupplyTotal -= 16;
	}
	if (unit->getType() == UnitTypes::Zerg_Hatchery) {
		m_SupplyTotal -= 2;
	}
}


/*
	But for a zerg building, complete is just complete.
*/
void BackManager::OnUnitComplete(Unit unit) {
	for (auto &p : m_Progress) {
		if (unit->getType() == p->type) {
			m_Progress.remove(p);
			break;
		}
	}
	if (unit->getType() == UnitTypes::Zerg_Hatchery) {
		m_SupplyTotal += 2;
	}
	if (unit->getType() == UnitTypes::Zerg_Overlord) {
		m_SupplyTotal += 16;
	}
	//Broodwar->sendText("Unit Create : %s", unit->getType().c_str());
}


int BackManager::CountUnitOfType(UnitType type) {
	int output = 0;
	for (auto &u : Broodwar->self()->getUnits()) {
		if (u->getType() == type) {
			output++;
		}
	}

	return output;
}


//____STATES___________________________________________________________________


void BackManager::SetState(int state) {
	switch (state) {
		default:
		case STATE_NULL:
			return;                 // RETURN HERE !! (Not in case, so invalid)
		case STATE_OPEN:
			StateInit_Open();
			break;
		case STATE_LOTS_OF_LINGS:
			StateInit_LotsOfLings();
			break;
	}

	m_READ_ONLY_State = state;
}


int BackManager::GetState() {
	return m_READ_ONLY_State;
}

char* BackManager::GetStateStr() {
	switch (m_READ_ONLY_State) {
		default:
		case STATE_NULL:
			return "NULL";
		case STATE_OPEN:
			return "OPEN";
		case STATE_LOTS_OF_LINGS:
			return "LOTS_OF_LINGS";
	}
}

/*
	Just add 5 plan of Drone
*/
void BackManager::StateInit_Open() {
	AddPlan(UnitTypes::Zerg_Drone, 5);

}

void BackManager::StateOnFrame_Open() {
	if (Broodwar->self()->supplyUsed() >= 18) {
		SetState(STATE_LOTS_OF_LINGS);
	}
}


/*
	Build 1 Overlord
	Build 1 Pool
	Build 5 Drones
	Build 3(6) Lings
	Build 1 Center
*/
void BackManager::StateInit_LotsOfLings() {
	AddPlan(UnitTypes::Zerg_Overlord);
	AddPlan(UnitTypes::Zerg_Drone, 3);
	AddPlan(UnitTypes::Zerg_Spawning_Pool);


}

/*
	If the money reaches 300, build one Center.
	If Supply is not enough, Build one.
	Then keep training lings.
*/
void BackManager::StateOnFrame_LotsOfLings() {

	if (Broodwar->self()->minerals() > 500 && CountUnitOfType(UnitTypes::Zerg_Hatchery) < 6) {
		bool hasPlan = false;
		for (auto &planNode: m_PlanList) {
			if (planNode->type == UnitTypes::Zerg_Hatchery) {
				hasPlan = true;
				break;
			}
		}
		if (!hasPlan) {
			AddPlan_Front(UnitTypes::Zerg_Hatchery);
		}
	}

	if (Broodwar->self()->supplyUsed() + 6 >= m_SupplyTotal) {
		if (!HasPlan(UnitTypes::Zerg_Overlord) && !HasProgress(UnitTypes::Zerg_Overlord)) {
			AddPlan_Front(UnitTypes::Zerg_Overlord);
		}
	}

	if (this->m_PlanList.empty()) {
		AddPlan(UnitTypes::Zerg_Zergling, 5);
		if (CountUnitOfType(UnitTypes::Zerg_Drone) < 30) {
			AddPlan(UnitTypes::Zerg_Drone, 1);
		}
	}

	if (FrontManager::GetInstance()->GetState() == STATE_ATTACK && GetMyResourcePoints().size() == 1) {
		if (!HasPlan(UnitTypes::Zerg_Hatchery)) {
			// Open this to play 2 base game.
			AddPlan(UnitTypes::Zerg_Hatchery, 1, GetClosestResourcePoint()->GetBuildLocation());
		}
	}

}

bool BackManager::HasPlan(UnitType type) {
	bool output = false;
	for (auto &p : m_PlanList) {
		if (p->type == type) {
			output = true;
			break;
		}
	}
	return output;
}

bool BackManager::HasProgress(UnitType type) {
	bool output = false;
	for (auto &p : m_Progress) {
		if (p->type == type) {
			output = true;
			break;
		}
	}
	return output;
}

/*
	Let the worker go to dig
	Check the idle out side
*/
void BackManager::WorkerGotoMinerals(UnitInterface* worker) {

	if (!worker->isIdle()) {
		return;
	}

	if (worker->isCarryingGas() || worker->isCarryingMinerals()) {
		worker->returnCargo();
	} else if (!worker->getPowerUp()) {
		if (!worker->gather(worker->getClosestUnit(IsMineralField || IsRefinery))) {
			// If the call fails, then print the last error message
			Broodwar << Broodwar->getLastError() << std::endl;
		} else {
			worker->returnCargo();
		}

	} 

	// Make each resourcePoint has same num of workers
	// One at every 100 frames, not so fast
	if (frameCount % 100 == 0) {
		int numOfResource = GetMyResourcePoints().size();
		if ( numOfResource > 1) {
			int numOfWorkers = CountUnitOfType(UnitTypes::Zerg_Drone);
			int numOfWorkers_Average = numOfWorkers / numOfResource;
			Broodwar->sendText("WorkersAverage=%d", numOfWorkers_Average);
			// find a point that has less workers;
			ResourcePoint* pSmallerPoint = NULL;
			ResourcePoint* pBiggerPoint = NULL;
			for (auto &p : GetMyResourcePoints()) {
				int p_NumOfWorkers = p->GetWorkers().size();
				if (!pSmallerPoint && p_NumOfWorkers - numOfWorkers_Average <= -3) {
					pSmallerPoint = p;
				} else if (!pBiggerPoint && p_NumOfWorkers - numOfWorkers_Average >= 3) {
					pBiggerPoint = p;
				}
			}
			if (pSmallerPoint && pBiggerPoint) {
				pBiggerPoint->GetWorkers().front()->move((Position)(pSmallerPoint->tilePosition));
				Broodwar->sendText("Send a worker to (%d,%d)", pSmallerPoint->tilePosition.x, pSmallerPoint->tilePosition.y);
			}
		}
	}
}

std::list<ResourcePoint*> BackManager::GetMyResourcePoints() {
	std::list<ResourcePoint*> output;
	for (auto &rp : m_ResourcePoints) {
		if (rp.isMine) {
			output.push_back(&rp);
		}
	}

	return output;
}

// Get Closest and Avaliable to use
ResourcePoint* BackManager::GetClosestResourcePoint() {
	ResourcePoint* pOutput = NULL;
	double minDis = std::numeric_limits<double>().max();
	for (auto &p : m_ResourcePoints) {
		if (!p.isMine && !p.isEnemy) {
			if (pOutput == NULL) {
				pOutput = &p;
				minDis = p.tilePosition.getDistance(Broodwar->self()->getStartLocation());
			} else {
				double dis = p.tilePosition.getDistance(Broodwar->self()->getStartLocation());
				if (dis < minDis) {
					minDis = dis;
					pOutput = &p;
				}
			}
		}
	}
	return pOutput;
}

