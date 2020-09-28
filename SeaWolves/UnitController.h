#pragma once

#ifndef __UnitController_h_
#define __UnitController_h_

#include <Ogre.h>
#include <stack>

#include "Unit.h"
#include "Projectile.h"

#include "PathFinding.h"
#include "GridUtils.h"

class UnitController
{
public:
	class SharedUnitReference
	{
	public:
		Unit* unit;
		bool available = true;

		int getUnitGridPositionX() {
			return this->unit->currentPos.x;
		}

		int getUnitGridPositionY() {
			return this->unit->currentPos.y;
		}
	};

	class SharedLocationReference
	{
	public:
		Ogre::Vector2 potentialLocation;
		bool available = true;

		int getLocationPositionX() {
			return potentialLocation.x;
		}

		int getLocationPositionY() {
			return potentialLocation.y;
		}
	};

	class PotentialUnitLocationReference
	{
	public:

		SharedUnitReference* unitRef;
		SharedLocationReference* locationRef;
		int distance = std::numeric_limits<int>::max();
	};

	class PotentialUnitLocation
	{
	public:

		Unit* unit;
		Ogre::Vector2 potentialLocation;
		int distance = std::numeric_limits<int>::max();
	};

	UnitController();
	virtual ~UnitController();

	//void assignUnitToFormationLocation(int width, int height, std::vector<Unit*> units, PathFinding* path);
	void fullLocationFormation(int width, int height, std::vector<Unit*> units, PathFinding* path);
	void priorityQueueFormation(int width, int height, std::vector<Unit*> units, PathFinding* path);
	void proximityLocationFormation(int width, int height, std::vector<Unit*> units, PathFinding* path);

private:
	//void insertNewRowPriorityMap(std::vector<std::vector<std::multimap<int, PotentialUnitLocation*>>>* rowPermutationPriorityQueue);

};



#endif __UnitController_h_