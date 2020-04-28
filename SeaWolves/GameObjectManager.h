#pragma once

#ifndef __GameObjectManager_h_
#define __GameObjectManager_h_

#include <Ogre.h>

#include "Unit.h"
#include "PathFinding.h"
#include "GridUtils.h"

class GameObjectManager
{
public:
	GameObjectManager();
	virtual ~GameObjectManager();

	void assignUnitToFormationLocation(int width, int height, std::vector<Unit*> units, PathFinding* path);

	class PotentialUnitLocation
	{
	public:
		Unit* unit;
		Ogre::Vector2 potentialLocation;
		int distance = std::numeric_limits<int>::max();
	};
};



#endif __GameObjectManager_h_