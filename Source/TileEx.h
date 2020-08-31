#pragma once
#include <BWAPI.h>

class TileEx {

public:

	BWAPI::TilePosition m_TilePosition;

	void OnInit(BWAPI::TilePosition tile);

	int GetKey();
	
	bool IsBuildable();

	// ���ͼ�鵽ĳ����λ�ľ���
	int DistanceToUnit(BWAPI::Unit unit);

	// ������ڵĿɽ���ͼ��
	// ������������ݹ�����ɽ��켯��
	std::list<TileEx> GetBuildableNeighbor();

};