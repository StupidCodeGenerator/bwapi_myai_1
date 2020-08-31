#pragma once
#include <BWAPI.h>
#include "UnitBrain.h"
#include "utils.h"
#include "TileEx.h"


class UnitBrain_CommandCenter : public UnitBrain {
public:

	int m_UnitID = NULL_UNIT_ID;

	std::map<int, TileEx> m_BuildableTilesMap;

	// �˻��ط�Χ�ڵĿ�
	std::list<int> m_Minerals;

	// �˻��ط�Χ�ڵ�Gas
	std::list<int> m_GasStations; 

	virtual int GetUnitID();

	virtual int OnInit(int unitID);

	void RefreshBuildableTiles();

	virtual int OnUpdate();

	virtual int OnDraw();
	
	// ��չm_BuildableTiles, һ��ʼҪ����һ��ͼ����Ϊ����
	// *�ݹ���þ���*
	void ExtendBuildableTiles_Recursive(TileEx seed, BWAPI::Unit cc);

};