#include "SteeringBehaviour.h"



SteeringBehaviour::SteeringBehaviour()
{
}


SteeringBehaviour::~SteeringBehaviour()
{
}

static int distanceTo(Ogre::Vector3 unit1, Ogre::Vector3 unit2) {
	int a = unit1.x - unit2.x;
	int b = unit1.z - unit2.z;

	int c = std::sqrt(a*a + b*b);
	return c;
}

static Ogre::Vector3* subtractVector(Ogre::Vector3 vec1, Ogre::Vector3 vec2) {
	int x, y, z;
	x = vec1.x - vec2.x;
	y = vec1.y - vec2.y;
	z = vec1.z - vec2.z;
	Ogre::Vector3* vec3 = new Ogre::Vector3(x, y, z);
	return vec3;
}
//----------------------------------------------------------------

Ogre::Vector3 SteeringBehaviour::seek(Unit* unit, Ogre::Vector3 destination) {
	//Desired change of location
	Ogre::Vector3 desired = destination.operator-=(unit->getPosition());

	//unit.distance = desired.normalise();

	//Desired velocity (move there at maximum speed)
	desired.operator*=(unit->walkSpeed / desired.length()); //TODO: maybe use squaredLength
														   //The velocity change we want
	Ogre::Vector3 force = desired - unit->velocity;
	//Convert to a force
	force.operator*=(unit->maxForce / unit->walkSpeed);

	return force;
}
//----------------------------------------------------------------

/*
Seperation keeps units from treading on each other. High seperation will create very jumpy movement. Counteract with alignment.
*/
Ogre::Vector3 SteeringBehaviour::seperation(Unit* unit, std::map<Ogre::String, Unit*>* units) {
	Ogre::Vector3 totalForce = Ogre::Vector3(0, 0, 0);
	int neighborsCount = 0;
	
	for (std::map<Ogre::String, Unit*>::iterator it = units->begin(); it != units->end(); ++it) {
		Unit* u = it->second;
		if (u->unitName != unit->unitName) {
			int distance = distanceTo(unit->getPosition(), u->getPosition());

			u->unitNode->getPosition();

			if (distance < unit->minSeperation && distance > 0) {
				Ogre::Vector3 vec1 = unit->getPosition();
				vec1.operator+(vec1);
				Ogre::Vector3 vec2 = u->getPosition();
				vec2.operator+(vec2);
				Ogre::Vector3* pushForce = subtractVector(unit->getPosition(), u->getPosition());
				totalForce.operator+=(pushForce->operator/(unit->seperationRadius));
				neighborsCount++;
				free(pushForce);
			}
		}
	}

	if (neighborsCount == 0) {
		return totalForce;
	}

	totalForce.operator/=(neighborsCount);
	totalForce.operator*=(unit->walkSpeed);

	return totalForce;
}
//----------------------------------------------------------------

/*
Keeps units clumped. Can easily overpower alignment and seperation.
*/
Ogre::Vector3 SteeringBehaviour::cohesion(Unit* unit, std::vector<Unit*>* units) {
	Ogre::Vector3 centerOfMass = unit->getPosition();
	int neighborsCount = 1;

	for (std::vector<Unit*>::iterator it = units->begin(); it != units->end(); ++it) {
		Unit* u = (*it);
		if (u->unitName != unit->unitName) {
			int distance = distanceTo(unit->getPosition(), u->getPosition());

			if (distance < unit->maxCohesion) {
				centerOfMass.operator+=(u->getPosition());
				neighborsCount++;
			}
		}
	}

	if (neighborsCount == 1) {
		return Ogre::Vector3::ZERO;
	}

	centerOfMass.operator/=(neighborsCount);
	return seek(unit, centerOfMass);
}
//----------------------------------------------------------------

bool SteeringBehaviour::halt(Unit* unit, std::vector<Unit*>* units) {
	Ogre::Vector3 centerOfMass = unit->getPosition();
	int neighborsCount = 1;

	for (std::vector<Unit*>::iterator it = units->begin(); it != units->end(); ++it) {
		Unit* u = (*it);
		if (u->unitName != unit->unitName) {
			int distance = distanceTo(unit->getPosition(), u->getPosition());

			if (distance < (unit->maxCohesion / 2)) {
				centerOfMass.operator+=(u->getPosition());
				neighborsCount++;
			}

			if (distance < (unit->maxCohesion / 4)) {
				if (u->unitAnimState->getAnimationName() == "Idle") {
					unit->haltTheGroup();
					return true;
				}
			}
		}
	}

	if (neighborsCount == 1) {
		return unit->hasArrived();
	}

	centerOfMass.operator/=(neighborsCount);
	Ogre::Vector2 position = GridUtils::cordNumericalFinder(centerOfMass);
	unit->debugPos1 = position;
	Ogre::Vector2 squareDest = GridUtils::cordNumericalFinder(unit->finalDestination);
	unit->debugPos2 = squareDest;
	if (position == squareDest) {
		unit->haltTheGroup();
		return true;
	}
	return false;
}
//----------------------------------------------------------------

/*
The higher alignment is, the smooth group movement will be. Alignment that is too high will result in wave like movement.
*/
Ogre::Vector3 SteeringBehaviour::alignment(Unit* unit, std::vector<Unit*>* units) {
	Ogre::Vector3* averageHeading = new Ogre::Vector3(0, 0, 0);
	int neighborsCount = 0;

	for (std::vector<Unit*>::iterator it = units->begin(); it != units->end(); ++it) {
		Unit* u = (*it);
		int distance = distanceTo(unit->getPosition(), u->getPosition());

		if (distance < unit->maxCohesion && unit->velocity.length() > 0) {
			averageHeading->operator+=(unit->velocity);
			neighborsCount++;
		}
	}

	if (neighborsCount == 0) {
		return Ogre::Vector3::ZERO;
	}

	averageHeading->operator/=(neighborsCount);
	averageHeading->y = 0;

	//Steer towards that heading
	Ogre::Vector3 desired = averageHeading->operator*=(unit->maxSpeed);
	Ogre::Vector3 force = desired.operator-=(unit->velocity);
	force.operator*=(unit->maxForce / unit->maxSpeed);
	free(averageHeading);
	return force;
}
//----------------------------------------------------------------