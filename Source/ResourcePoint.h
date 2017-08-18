/*
	A tile tilePosition that have minerals and gas.
	Will not have a gas list but a bool that tells there's gas or not here.
*/

#pragma once

#include "BWAPI.h";

// Radius of the resouce point ( in Tile )
#define RESOURCE_POINT_R     30 

using namespace BWAPI;

class ResourcePoint{

public :

	bool isMine;

	bool isEnemy;

	double tileDistanceFromMe;

	ResourcePoint(TilePosition firstPoint);

	// That may be the first mineral or gas 's tilePosition
	TilePosition tilePosition;

	std::list<TilePosition> mineralPositions;

	// Check if some mineral field is in range of this point
	bool AddThisPointIfInRange(UnitInterface* pUnit);

	TilePosition* GetBuildLocation();

	std::list<UnitInterface*> GetWorkers();

private:

	TilePosition* pBestCache;

	double GetAverageDistance(TilePosition* pPosition);

};