#pragma once
#include <BWAPI.h>
//
//	��Դ������
//
//	��������SCV�ɿ����, �Լ���Դ�����. ������ҪԤ������Ǯ, ��Ҫ����Ƭ��, �����ж��ٿ�, ����ʲô�ط�, 
//	SCV���Բ�ѯ�����Դ�������������һ�����ŵĿ���вɼ��ȵ�. 
//

class MineralExtend;

class ResourceManager {

public:

	std::map<int, MineralExtend*> m_MineralsMap;

	int m_SystemTime;

	int OnInit();

	int OnUpdate();

	int CreateMineralExtend(BWAPI::Unit unit);

	// ���һ��SCV�������ٵĿ�.
	// ���SCV������һ��, ����һ�����������
	// maxDistance ��ʾ��������, ����������������, ����������
	// ��ΪSCVԶ�̲ɿ���һ���Ƚ�������߼�
	int FindMineralUnitID_of_MinSCV(int maxDistance, BWAPI::Unit scvUnit);

	std::list<int> FindMineralUnitInRange(BWAPI::Position center, int radius);
	std::list<int> FindGasUnitInRange(BWAPI::Position center, int radius);

	int AddSCV_of_Mineral(int mineralUnitID);       // ֪ͨĳ���SCV��ռ��
	int RemoveSCV_of_Mineral(int mineralUnitID);    // ֪ͨĳ���SCV����һ����ռ

	void OnDraw();
};