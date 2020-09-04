#include "TileEx.h"
#include <BWAPI.h>

void TileEx::OnInit(BWAPI::TilePosition tile) { 
	m_TilePosition = tile; 
}

int TileEx::GetKey() {
	return (m_TilePosition.x << 8) + m_TilePosition.y;
}

bool TileEx::IsBuildable() {
	return BWAPI::Broodwar->isBuildable(m_TilePosition);
}

int TileEx::DistanceToUnit(BWAPI::Unit unit) {
	if(!unit){
		return 0;
	}
	BWAPI::Position p = BWAPI::Position(m_TilePosition.x * TILE_SIZE, m_TilePosition.y * TILE_SIZE);
	return unit->getDistance(p);
}

std::list<TileEx> TileEx::GetBuildableNeighbor() {

	BWAPI::TilePosition top = BWAPI::TilePosition(m_TilePosition.x, m_TilePosition.y - 1);
	BWAPI::TilePosition bottom = BWAPI::TilePosition(m_TilePosition.x, m_TilePosition.y + 1);
	BWAPI::TilePosition left = BWAPI::TilePosition(m_TilePosition.x - 1, m_TilePosition.y);
	BWAPI::TilePosition right = BWAPI::TilePosition(m_TilePosition.x + 1, m_TilePosition.y);

	TileEx topEx, bottomEx, leftEx, rightEx;
	topEx.OnInit(top);
	bottomEx.OnInit(bottom);
	leftEx.OnInit(left);
	rightEx.OnInit(right);

	std::list<TileEx> output;
	if(topEx.IsBuildable()) output.push_back(topEx);
	if(bottomEx.IsBuildable()) output.push_back(bottomEx);
	if(leftEx.IsBuildable()) output.push_back(leftEx);
	if(rightEx.IsBuildable()) output.push_back(rightEx);

	return output;
}

bool TileEx::operator < (TileEx other) {
	return this->m_DistanceToCommandCenter < other.m_DistanceToCommandCenter;
}

bool TileEx::operator > (TileEx other) {
	return this->m_DistanceToCommandCenter > other.m_DistanceToCommandCenter;
}