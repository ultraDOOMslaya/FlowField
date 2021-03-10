#include "Player.h"



Player::Player()
	: queuedAttackMove(false)
{
}


Player::~Player()
{
}

/*
void Player::focusUnits(Ogre::SceneQueryResult& result, std::map<Ogre::String, Unit>* units) {
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

void Player::focusUnits(Ogre::SceneQueryResult& result) {
	clearUnitQueue();

	std::map<Ogre::String, Unit*>::iterator itTree;
	Ogre::SceneQueryResultMovableList::iterator it;
	for (it = result.movables.begin(); it != result.movables.end(); ++it) {
		if ((*it)->getQueryFlags() == Constants::buildingQueryMask) {
			focusBuilding(mRedBuilding);
			
		}
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

void Player::focusUnit(Unit* unit) {
	clearUnitQueue();
	addToQueue(unit);
	assignToGroup();
}
//----------------------------------------------------------------

void Player::focusBuilding(Building* building) {
	mFocusedBuilding = nullptr;
	mFocusedBuilding = building;
	mFocusedBuilding->selected();
}
//----------------------------------------------------------------

void Player::addToQueue(Unit* unit) {
	unit->selected();
	unitQueue.push_back(unit);
}
//----------------------------------------------------------------

void Player::assignToGroup() {
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

/**
* Remove unit from all groups, delete the units Ogre references, and free the memory
**/
void Player::cullUnit(Unit* unit) {
	for (auto group = groups.begin(); group != groups.end(); ++group) {
		for (auto unitInstance = group->begin(); unitInstance != group->end(); ++unitInstance) {
			if ((*unitInstance) == unit) {
				group->erase(unitInstance--);
			}			
		}
	}

	for (auto it = myArmy.begin(); it != myArmy.end(); ++it) {
		Unit* unitInArmy = it->second;
		if (unitInArmy == unit) {
			myArmy.erase(it--);
		}
	}
}
//----------------------------------------------------------------

void Player::clearUnitQueue() {
	std::vector<Unit*>::iterator ia;
	for (ia = unitQueue.begin(); ia != unitQueue.end(); ia++) {
		(*ia)->unselected();
	}
	unitQueue.clear();
}
//----------------------------------------------------------------

void Player::clearSelectedBuilding() {
	mFocusedBuilding->unselected();
	mFocusedBuilding = nullptr;
}
//----------------------------------------------------------------

bool Player::hasUnitInArmy(Ogre::String unitName) {
	if (myArmy.find(unitName) != myArmy.end()) {
		return true;
	}
	return false;
}
//----------------------------------------------------------------

void Player::attack() {
	for (std::vector<Unit*>::iterator unit = unitQueue.begin(); unit != unitQueue.end(); ++unit) {
		(*unit)->attackingTarget();
	}
}
//----------------------------------------------------------------

void Player::attack(Unit* target) {
	for (std::vector<Unit*>::iterator unit = unitQueue.begin(); unit != unitQueue.end(); ++unit) {
		(*unit)->trekking = false;
		(*unit)->setTarget(target);
		(*unit)->mState = Unit::STATE_SEEKING;
		(*unit)->mSeekingState->enter(**unit);
	}
}
//----------------------------------------------------------------

void Player::attackMove() {
	//TODO can we remove most of these assignments?
	for (std::vector<Unit*>::iterator unit = unitQueue.begin(); unit != unitQueue.end(); ++unit) {
		(*unit)->attacking = true;
		(*unit)->hunting = true;
		(*unit)->trekking = false;
		(*unit)->mState = Unit::STATE_AGGRESSIVE;
		(*unit)->mAggressiveState->enter(**unit);
	}
}
//-----------------------------------------------------=-----------

//TODO move this to a generic util class
/*
 *Find the average of the given unit queues position
 */
Ogre::Vector3 Player::unitGroupConglomerate() {
	float x = 0;
	float y = 0;

	for (std::vector<Unit*>::iterator it = unitQueue.begin(); it != unitQueue.end(); ++it) {
		x += (*it)->getB2DPosition().x;
		y += (*it)->getB2DPosition().y;
	}

	x /= unitQueue.size();
	y /= unitQueue.size();
	Ogre::Vector3 conglomerate = Ogre::Vector3(x, 0, y);
	return conglomerate;
}