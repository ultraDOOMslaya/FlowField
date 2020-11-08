#pragma once

#ifndef __UnitController_h_
#define __UnitController_h_

#include <Ogre.h>
#include <stack>

#include "Player.h"
#include "Projectile.h"
#include "MagicAttack.h"
#include "Unit.h"

#include "PathFinding.h"
#include "GridUtils.h"

class Player;
class Unit;
class Projectile;
class MagicAttack;

class UnitController
{
public:
	class SharedUnitReference
	{
	public:
		Unit* unit;
		bool available = true;

		int getUnitGridPositionX();
		/*int getUnitGridPositionX() {
			return this->unit->currentPos.x;
		}*/

		int getUnitGridPositionY();
		/*int getUnitGridPositionY() {
			return this->unit->currentPos.y;
		}*/
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

	UnitController(std::map<Ogre::String, Unit*>* units, std::vector<Player*>* players, std::vector<Projectile*>* projectiles, std::vector<MagicAttack*>* magicAttacks);
	virtual ~UnitController();

	//void assignUnitToFormationLocation(int width, int height, std::vector<Unit*> units, PathFinding* path);
	void huntForTarget(Unit* unit);
	void seekTarget(Unit* unit);
	void spawnSpellAction(Unit* unit);
	void clearTargets(Unit* expiredTarget);

	void fullLocationFormation(int width, int height, std::vector<Unit*> units, PathFinding* path);
	void priorityQueueFormation(int width, int height, std::vector<Unit*> units, PathFinding* path);
	void proximityLocationFormation(int width, int height, std::vector<Unit*> units, PathFinding* path);

private:
	std::map<Ogre::String, Unit*>*					allUnits;
	std::vector<Player*>*						    allPlayers;
	std::vector<Projectile*>*						allProjectiles;
	std::vector<MagicAttack*>*						allMagicAttacks;

};



#endif __UnitController_h_