#pragma once
#include <BWAPI.h>
#include <Windows.h>

#define FIND_UNIT(X) (BWAPI::Broodwar->getUnit((X)))
#define SELF (BWAPI::Broodwar->self())
#define SCREEN_X(X) ((X) - BWAPI::Broodwar->getScreenPosition().x)
#define SCREEN_Y(Y) ((Y) - BWAPI::Broodwar->getScreenPosition().y)

#define MINERALS_MAP AI_MODULE->m_pResourceManager->m_MineralsMap

using namespace std;

// Remember not to use "Broodwar" in any global class constructor!



class UnitBrainManager;
class ResourceManager;

class MyAiModule : public BWAPI::AIModule
{
public:

	UnitBrainManager* m_pUnitBrainManager;

	ResourceManager* m_pResourceManager;

	std::list<int> m_SupplyGrowthList_10s;                     // 10���Ӽ�һ��,�˿�����
	std::list<int> m_MineGrowthList_10s;                       // 10���Ӽ�һ��,ˮ������

	//	���Բ���ĳ��, �Ҳ�������NULL(������������ж�)
	BWAPI::Unit FindMinerals(int unitID);

	int GetMoney(); // Minerals
	int GetGas();
	int GetSupply();  // Remains

	//	���ָ�����͵�Unit������
	int GetUnitCount(BWAPI::UnitType unitType);

	DWORD m_LastUpdateTime = 0;      // ��ֻ��WindowsAPI������DWORD, �������������
	// 
	//	�� onFrame ����,�����������������������0.1�����һ��
	//	���õ�̫���˻�����ܶ�����.
	//
	int OnUpdate();

	// 10���ӽ���һ�ε�Update
	DWORD m_Last_10s_UpdateTime = 0; // ��һ�ε��ô˺�����ʱ��
	int OnUpdate_10s();


	// Virtual functions for cm_UnitMap leave these as they are.
	virtual void onStart();
	virtual void onEnd(bool isWinner);
	virtual void onFrame();
	virtual void onSendText(std::string text);
	virtual void onReceiveText(BWAPI::Player player, std::string text);
	virtual void onPlayerLeft(BWAPI::Player player);
	virtual void onNukeDetect(BWAPI::Position target);
	virtual void onUnitDiscover(BWAPI::Unit unit);
	virtual void onUnitEvade(BWAPI::Unit unit);
	virtual void onUnitShow(BWAPI::Unit unit);
	virtual void onUnitHide(BWAPI::Unit unit);
	virtual void onUnitCreate(BWAPI::Unit unit);
	virtual void onUnitDestroy(BWAPI::Unit unit);
	virtual void onUnitMorph(BWAPI::Unit unit);
	virtual void onUnitRenegade(BWAPI::Unit unit);
	virtual void onSaveGame(std::string gameName);
	virtual void onUnitComplete(BWAPI::Unit unit);
	// Everything below this line is safe to modify.

	void LogOnScreen();

};


extern MyAiModule* AI_MODULE;
