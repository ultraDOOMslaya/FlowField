#include "CombatBehaviour.h"


void CombatBehaviour::seekTarget(std::map<Ogre::String, Unit*>* units, PlayerManager* activePlayer, std::vector<PlayerManager*> players, Unit* unit) {
	for (std::map<Ogre::String, Unit*>::iterator it = units->begin(); it != units->end(); ++it) {
		Unit* potentialTarget = it->second;
		
		int proximity_x = std::pow((unit->getPosition().x - potentialTarget->getPosition().x), 2);
		int proximity_y = std::pow((unit->getPosition().y - potentialTarget->getPosition().y), 2);
		int proximity = proximity_x + proximity_y;

		if (proximity < std::pow(unit->targetRadius, 2)) {
			if (PlayerUtils::determineStatus(activePlayer, players, potentialTarget) == PlayerRelationshipStatus::HOSTILE) {
				unit->setLooseTarget(potentialTarget, proximity);
			}
		}
	}

	if (unit->mTarget == NULL && !unit->isAnimation("Walk")) {
		unit->attacking = false;
		unit->animate("Idle");
	}
	else if (unit->mTarget == NULL) {
		unit->attacking = false;
	}
}
//----------------------------------------------------------------

void CombatBehaviour::huntForTarget(std::map<Ogre::String, Unit*>* units, PlayerManager* activePlayer, std::vector<PlayerManager*> players, Unit* unit) {
	for (std::map<Ogre::String, Unit*>::iterator it = units->begin(); it != units->end(); ++it) {
		Unit* potentialTarget = it->second;
		//int distance = std::abs(unit->getPosition().length() - potentialTarget->getPosition().length());

		int proximity_x = std::pow((unit->getPosition().x - potentialTarget->getPosition().x), 2);
		int proximity_y = std::pow((unit->getPosition().y - potentialTarget->getPosition().y), 2);
		int proximity = proximity_x + proximity_y;

		if (proximity < std::pow(unit->targetRadius, 2)) {
			if (PlayerUtils::determineStatus(activePlayer, players, potentialTarget) == PlayerRelationshipStatus::HOSTILE) {
				int currentDistance = std::numeric_limits<int>::max();
				if (unit->mTarget != NULL) {
					//currentDistance = std::abs(unit->getPosition().length() - unit->mTarget->getPosition().length());
					int proximity_x = std::pow((unit->getPosition().x - unit->mTarget->getPosition().x), 2);
					int proximity_y = std::pow((unit->getPosition().y - unit->mTarget->getPosition().y), 2);
					currentDistance = proximity_x + proximity_y;
				}
				//if (distance < currentDistance) {
				if (proximity < currentDistance) {
					unit->setLooseTarget(potentialTarget, proximity);
				}
			}
		}
	}
}
//----------------------------------------------------------------

