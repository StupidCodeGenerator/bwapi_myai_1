#include "ResourcePoint.h"
#include "InformationManager.h"

using namespace BWAPI;

#define RP_RADIUS 20    // Range of a resource point

ResourcePoint::ResourcePoint(TilePosition firstPoint) {
	this->tilePosition = *(new TilePosition(firstPoint));
	this->mineralPositions.push_back(tilePosition);
	this->isMine = false;
	pBestCache = NULL;
}

/*
	Check if the mineral field is in this range
*/
bool ResourcePoint::AddThisPointIfInRange(UnitInterface* pUnit) {
	for (auto &mp : mineralPositions) {
		if (mp.getDistance(pUnit->getTilePosition()) < RP_RADIUS) {
			if (pUnit->getType().isMineralField()) {
				mineralPositions.push_back(*(new TilePosition(pUnit->getTilePosition())));
				return true;
			}
		}
	}

	return false;   // not found, new resource point
}


TilePosition* ResourcePoint::GetBuildLocation() {

	for (int i = 0; i < 100; i++) {
		double minDistance = -1;
		for (auto &tp : this->mineralPositions) {
			if (minDistance < 0) {
				pBestCache = new TilePosition(Broodwar->getBuildLocation(UnitTypes::Zerg_Hatchery, tp, 20));
				minDistance = GetAverageDistance(pBestCache);
			} else {
				TilePosition* pNewPos = new TilePosition(Broodwar->getBuildLocation(UnitTypes::Zerg_Hatchery, tp, 20));
				double distance = GetAverageDistance(pNewPos);
				if (distance < minDistance) {
					minDistance = distance;
					pBestCache = pNewPos;
				}
			}
		}
		// Check buildable
		if (Broodwar->isBuildable(*pBestCache)) {
			break;
		}
	}

	return pBestCache;
}

std::list<UnitInterface*> ResourcePoint::GetWorkers() {
	std::list<UnitInterface*> output;
	for (auto &u : Broodwar->self()->getUnits()) {
		if (u->getType() == UnitTypes::Zerg_Drone && u->getTilePosition().getDistance(this->tilePosition) < 20) {
			output.push_back(u);
		}
	}

	return output;
}

double ResourcePoint::GetAverageDistance(TilePosition* pPosition) {
	double outputTotal = 0;

	for (auto &tp : mineralPositions) {
		outputTotal += tp.getDistance(*pPosition);
	}

	return outputTotal / mineralPositions.size();
}