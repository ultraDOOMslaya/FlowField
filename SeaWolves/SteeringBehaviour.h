#pragma once

#include "Ogre.h"

#include "Unit.h"
#include "GridUtils.h"
#include "PlayerUtils.h"

class SteeringBehaviour
{
public:

	static Ogre::Vector3 seek(Unit* unit, Ogre::Vector3 destination);
	static b2Vec2 seek(Unit* unit, b2Vec2 destination);
	static b2Vec2 b2seperation(Unit* unit, std::map<Ogre::String, Unit*>* units, Player* activePlayer, std::vector<Player*> players);
	static Ogre::Vector3 seperation(Unit* unit, std::map<Ogre::String, Unit*>* units, Player* activePlayer, std::vector<Player*> players);
	static Ogre::Vector3 seperation(Unit* unit, std::map<Ogre::String, Unit*>* units);
	static Ogre::Vector3 cohesion(Unit* unit, std::vector<Unit*>* units);
	static bool halt(Unit* unit, std::vector<Unit*>* units);
	static Ogre::Vector3 alignment(Unit* unit, std::vector<Unit*>* units);
	static Ogre::Vector3 staticObjectCollisionForceApplier(Unit* unit);
	static Ogre::Vector3 unitGroupConglomerate(std::vector<Unit*>* units);

	SteeringBehaviour();
	~SteeringBehaviour();
};

