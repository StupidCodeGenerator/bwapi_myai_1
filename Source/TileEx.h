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

	// CommandCenterר�õ�������, ��Ҫ�ȴ洢�˵�CommandCenter�ľ���, Ȼ���������
	// ������ú�, �����Ч
	
	int m_DistanceToCommandCenter = 0xFFFF;                  // ��CommandCenter�ľ���
	
	// ����ʹ�õĲ���������
	bool operator < (TileEx other);
	bool operator > (TileEx other);

};