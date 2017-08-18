#pragma once
#include "BWAPI.h"
#include "ResourcePoint.h"

using namespace BWAPI;
using namespace Filter;


#define    STATE_NULL              0
#define    STATE_OPEN              1
#define    STATE_LOTS_OF_LINGS     2


typedef struct PlanNodeStruct{

	UnitInterface*  pBuilder;     // Can be null in some case
	UnitType        type;
	TilePosition*   pTilePosition;

} PlanNode;


class BackManager {

public:
	
	/*
		When you first time want to build or train sth, It will appear in 
		planlist. then when you send someone to do that job, It will pop from 
		planlist and goto progress.
		When that unit was morph or create, Remove that in the progress
	*/
	std::list<PlanNode*>      m_PlanList;
	std::list<PlanNode*>      m_Progress;       // <typeID, num>

	int m_SupplyTotal;   // the API's function has delay, so I will count them manually

	std::list<ResourcePoint> m_ResourcePoints;

	// Singleton
	static BackManager* GetInstance() {
		if (!m_pInstance) {
			m_pInstance = new BackManager();
		}
		return m_pInstance;
	}

	// The Plan System
	void AddPlan(UnitType type, int num, TilePosition* pTilePosition);
	void AddPlan(UnitType type, int num);
	void AddPlan(UnitType type);

	void AddPlan_Front(UnitType type, int num, TilePosition* pTilePosition);
	void AddPlan_Front(UnitType type, int num);
	void AddPlan_Front(UnitType type);

	bool HasPlan(UnitType type);
	bool HasProgress(UnitType type);

	// Events
	void OnStart();
	void OnFrame();
	void OnUnitMorph(Unit unit);
	void OnUnitDestory(Unit unit);
	void OnUnitComplete(Unit unit);
	int CountUnitOfType(UnitType type);

	// States

	void SetState(int state);
	int GetState();
	char* GetStateStr();
	void StateInit_Open();
	void StateOnFrame_Open();
	void StateInit_LotsOfLings();
	void StateOnFrame_LotsOfLings();

	// Others
	void WorkerGotoMinerals(UnitInterface* worker);
	std::list<ResourcePoint*> GetMyResourcePoints();
	ResourcePoint* GetClosestResourcePoint();

private:

	unsigned int frameCount = 0;

	int m_READ_ONLY_State; // Do not modify this variable manually!

	BackManager();
	~BackManager();
	
	static BackManager*    m_pInstance;

	void UpdatePlan();

};