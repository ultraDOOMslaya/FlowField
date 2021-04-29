#pragma once

#include "Ogre.h"

#include "Unit.h"
#include "Constants.h"
#include "PlayerRelationship.h"
#include "Building.h"

class Unit;

class Player
{
public:
	Player();
	~Player();
	//virtual void focusUnits(Ogre::SceneQueryResult& result, std::map<Ogre::String, Unit>* units);
	virtual void focusUnits(Ogre::SceneQueryResult& result);
	virtual void focusUnit(Unit* unit);
	virtual void focusBuilding(Building* building);
	virtual void addToQueue(Unit* unit);
	virtual void addToBuildingQueue(Building* building);
	virtual void assignToGroup();
	virtual void clearUnitQueue();
	virtual void clearBuildingQueue();
	virtual void cullUnit(Unit* unit);
	virtual void harvest(NaturalResource* target);
	virtual void attack();
	virtual void attack(Unit* target);
	virtual void attackMove();
	virtual bool hasUnitInArmy(Ogre::String unitName);
	virtual Ogre::Vector3 unitGroupConglomerate();
	

	std::vector<Unit*>					unitQueue;		//Actively being controlled
	std::vector<Building*>				buildingQueue;	
	std::vector<std::vector<Unit*>>		groups;			//Manage groups with previous orders
	std::vector<Unit*>					unitGroup;		//Have previous orders
	std::vector<Unit*>					renegeGroup;	//Units switching groups
	int									mLastUnitId;

	std::map<Ogre::String, Unit*>		myArmy;
	std::map<Ogre::String, Building*>   myBuildings;
	PlayerRelationship*					relationship;
	bool								queuedAttackMove;
	int									playerId;
	
};

