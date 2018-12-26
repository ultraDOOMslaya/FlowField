#include "PlayerManager.h"



PlayerManager::PlayerManager()
	: queuedAttackMove(false)
{
}


PlayerManager::~PlayerManager()
{
}

/*
void PlayerManager::focusUnits(Ogre::SceneQueryResult& result, std::map<Ogre::String, Unit>* units) {
	clearUnitQueue();

	std::map<Ogre::String, Unit>::iterator itTree;
	Ogre::SceneQueryResultMovableList::iterator it;
	for (it = result.movables.begin(); it != result.movables.end(); ++it) {
		if ((*it)->getQueryFlags() == Constants::unitQueryMask) {
			itTree = units->find((*it)->getParentSceneNode()->getName());
			addToQueue(&itTree->second);
		}
	}
	assignToGroup();
}*/
//----------------------------------------------------------------

void PlayerManager::focusUnits(Ogre::SceneQueryResult& result) {
	clearUnitQueue();

	std::map<Ogre::String, Unit*>::iterator itTree;
	Ogre::SceneQueryResultMovableList::iterator it;
	for (it = result.movables.begin(); it != result.movables.end(); ++it) {
		if ((*it)->getQueryFlags() == Constants::unitQueryMask) {
			if (hasUnitInArmy((*it)->getParentSceneNode()->getName())) {
				itTree = myArmy.find((*it)->getParentSceneNode()->getName());
				addToQueue(itTree->second);
			}
		}
	}
	assignToGroup();
}
//----------------------------------------------------------------

void PlayerManager::focusUnit(Unit* unit) {
	clearUnitQueue();
	addToQueue(unit);
	assignToGroup();
}
//----------------------------------------------------------------

void PlayerManager::addToQueue(Unit* unit) {
	unit->selected();
	unitQueue.push_back(unit);
}
//----------------------------------------------------------------

void PlayerManager::assignToGroup() {
	std::vector<Unit*>* newUnitGroup = new std::vector<Unit*>();
	for (std::vector<Unit*>::iterator unit = unitQueue.begin(); unit != unitQueue.end(); ++unit) {
		if ((*unit)->group != NULL) {
			(*unit)->group->erase(std::remove((*unit)->group->begin(), (*unit)->group->end(), *unit), (*unit)->group->end());
			if ((*unit)->group->size() == 0) {
				delete (*unit)->group;
			}
		}
		newUnitGroup->push_back(*unit);
		(*unit)->group = newUnitGroup;
	}
}
//----------------------------------------------------------------

void PlayerManager::clearUnitQueue() {
	std::vector<Unit*>::iterator ia;
	for (ia = unitQueue.begin(); ia != unitQueue.end(); ia++) {
		(*ia)->unselected();
	}
	unitQueue.clear();
}
//----------------------------------------------------------------

bool PlayerManager::hasUnitInArmy(Ogre::String unitName) {
	if (myArmy.find(unitName) != myArmy.end()) {
		return true;
	}
	return false;
}
//----------------------------------------------------------------

void PlayerManager::attack() {
	for (std::vector<Unit*>::iterator unit = unitQueue.begin(); unit != unitQueue.end(); ++unit) {
		(*unit)->attackTarget();
	}
}
//----------------------------------------------------------------

void PlayerManager::attack(Unit* target) {
	for (std::vector<Unit*>::iterator unit = unitQueue.begin(); unit != unitQueue.end(); ++unit) {
		(*unit)->attack(target);
	}
}
//----------------------------------------------------------------

void PlayerManager::attackMove() {
	for (std::vector<Unit*>::iterator unit = unitQueue.begin(); unit != unitQueue.end(); ++unit) {
		(*unit)->attacking = true;
	}
}
//----------------------------------------------------------------