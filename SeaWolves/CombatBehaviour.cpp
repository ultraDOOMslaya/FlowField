#include "CombatBehaviour.h"


void CombatBehaviour::seekTarget(std::map<Ogre::String, Unit*>* units, PlayerManager* activePlayer, std::vector<PlayerManager*> players, Unit* unit) {
	for (std::map<Ogre::String, Unit*>::iterator it = units->begin(); it != units->end(); ++it) {
		Unit* potentialTarget = it->second;
		//int distance = std::abs(unit->getPosition().squaredLength() - potentialTarget->getPosition().squaredLength());
		int distance = std::abs(unit->getPosition().length() - potentialTarget->getPosition().length());
		//if (distance < std::pow(unit->targetRadius, 2)) {
		if (distance < unit->targetRadius) {
			if (PlayerUtils::determineStatus(activePlayer, players, potentialTarget) == PlayerRelationshipStatus::HOSTILE) {
				unit->setLooseTarget(potentialTarget, distance);
			}

		}
	}
}
//----------------------------------------------------------------

void CombatBehaviour::huntForTarget(std::map<Ogre::String, Unit*>* units, PlayerManager* activePlayer, std::vector<PlayerManager*> players, Unit* unit) {
	for (std::map<Ogre::String, Unit*>::iterator it = units->begin(); it != units->end(); ++it) {
		Unit* potentialTarget = it->second;
		//int distance = std::abs(unit->getPosition().squaredLength() - potentialTarget->getPosition().squaredLength());
		int distance = std::abs(unit->getPosition().length() - potentialTarget->getPosition().length());
		//if (distance < std::pow(unit->targetRadius, 2)) {
		if (distance < unit->targetRadius) {
			if (PlayerUtils::determineStatus(activePlayer, players, potentialTarget) == PlayerRelationshipStatus::HOSTILE) {
				//int currentDistance = std::abs(unit->getPosition().squaredLength() - unit->mTarget->getPosition().squaredLength());
				int currentDistance = std::abs(unit->getPosition().length() - unit->mTarget->getPosition().length());
				if (distance < currentDistance) {
					unit->setLooseTarget(potentialTarget, distance);
				}
			}
		}
	}
}
//----------------------------------------------------------------

