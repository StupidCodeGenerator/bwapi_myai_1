#pragma once
#include <BWAPI.h>
#include <Windows.h>

using namespace std;

// Remember not to use "Broodwar" in any global class constructor!

class UnitBrainManager;

class MyAiModule : public BWAPI::AIModule
{
public:

	UnitBrainManager* m_pUnitBrainManager;

	//	Unit表
	//
	//	事件驱动, UnitCreate的时候 +
	//	UnitDestory的时候 -
	std::map<int, BWAPI::Unit> m_UnitMap;

	//	尝试查找Unit,找不到返回NULL(UNIT本来就是指针类型)
	BWAPI::Unit FindUnit(int unitID);

	int GetMoney(); // Minerals
	int GetGas();
	int GetSupply();  // Remains

	//	获得指定类型的Unit的数量
	int GetUnitCount(BWAPI::UnitType unitType);

	DWORD m_LastUpdateTime = 0;      // 我只在WindowsAPI里面用DWORD, 我讨厌这个类型
	// 
	//	由 onFrame 激发,但是区别在于这个被限制在0.1秒调用一次
	//	调用的太快了会产生很多问题.
	//
	int OnUpdate();

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
