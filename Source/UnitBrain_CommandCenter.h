#pragma once
#include <BWAPI.h>
#include "UnitBrain.h"
#include "utils.h"
#include "TileEx.h"


class UnitBrain_CommandCenter : public UnitBrain {
public:

	int m_UnitID = NULL_UNIT_ID;

	std::map<int, TileEx> m_BuildableTilesMap;            // ��ֵ�Է�ʽ�洢��ͼ���, �������ɽ�����һЩ��Ҫ��ֵ��ϵ�ĵط�
	std::list<TileEx> m_SortedBuildableTilesList;         // ������ͼ���, �������ٻ�ý����

	std::map<int, TileEx> m_OrderedTilesMap;              // ��Ԥ����ͼ��, һ���������ĳ��SCV������ͼ��Ҫ��ȥ������

	// �˻��ط�Χ�ڵĿ�
	std::list<int> m_Minerals;

	// �˻��ط�Χ�ڵ�Gas
	std::list<int> m_GasStations; 

	virtual int GetUnitID();

	virtual int OnInit(int unitID);

	void RefreshBuildableTiles();

	// ��Center��ȡһ�����Խ����λ��
	BWAPI::TilePosition GetBuildPosition(int tileWidth, int tileHeight);

	bool IsAreaBuildable(TileEx tile, int tileWidth, int tileHeight);
	bool IsTileBuildable(int tileKey);
	std::list<int> GetTileKeysIfAreaBuildable(TileEx tile, int tileWidth, int tileHeight);

	virtual int OnUpdate();

	virtual int OnDraw();
	
	// ��չm_BuildableTiles, һ��ʼҪ����һ��ͼ����Ϊ����
	// *�ݹ���þ���*
	void ExtendBuildableTiles_Recursive(TileEx seed, BWAPI::Unit cc);

};