#pragma once

#include <BWAPI.h>
#include "utils.h"

//
//	ˮ����չ�߼�, �����Ƿ�ĳ��SCV���ϵ�. 
//
class MineralExtend {

private:
	
	int m_SCV_Count = 0;           // ��ʾ��ǰ�м���SCV�����������(�����Ѿ��������˾���, ������û�п�ʼ��)
	
public:

	int m_UnitID = NULL_UNIT_ID;

	void OnInit(int unitID);
	
	void AddSCV();                 // ����SCV����

	void RemoveSCV();              // ����SCV����

	int GetSCVCount();
};
