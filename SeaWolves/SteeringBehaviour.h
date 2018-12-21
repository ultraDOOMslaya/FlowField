#pragma once

#include "Ogre.h"

#include "Unit.h"
#include "GridUtils.h"

class SteeringBehaviour
{
public:

	static Ogre::Vector3 seek(Unit unit, Ogre::Vector3 destination);
	static Ogre::Vector3 seperation(Unit unit, std::map<Ogre::String, Unit>* units);
	static Ogre::Vector3 cohesion(Unit unit, std::vector<Unit*>* units);
	static bool halt(Unit* unit, std::vector<Unit*>* units);
	static Ogre::Vector3 alignment(Unit unit, std::vector<Unit*>* units);

	SteeringBehaviour();
	~SteeringBehaviour();
};

