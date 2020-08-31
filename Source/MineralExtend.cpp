#include "MineralExtend.h"
#include <BWAPI.h>
#include "utils.h"


void MineralExtend::OnInit(int unitID) {
	m_UnitID = unitID;
	m_SCV_Count = 0;
}


void MineralExtend::AddSCV() {
	m_SCV_Count++;
}


void MineralExtend::RemoveSCV() {
	if (m_SCV_Count > 0) {
		m_SCV_Count--;
	}
}


int MineralExtend::GetSCVCount() {
	return m_SCV_Count;
}