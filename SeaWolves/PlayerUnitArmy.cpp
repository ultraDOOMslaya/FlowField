#include "PlayerUnitArmy.h"



PlayerUnitArmy::PlayerUnitArmy()
{
}


PlayerUnitArmy::PlayerUnitArmy(std::vector<Unit*> unitArmy) {
	//TODO finish this
}


PlayerUnitArmy::PlayerUnitArmy(std::vector<Unit> unitArmy) {
	//TODO finish this
}


PlayerUnitArmy::~PlayerUnitArmy()
{
}


void PlayerUnitArmy::setUnitArmy(std::vector<Unit*> unitArmy) {
	for (std::vector<Unit*>::iterator unit = unitArmy.begin(); unit < unitArmy.end(); unit++) {
		mUnitArmy.insert(std::make_pair((*unit)->getName(), *unit));
	}
}
//----------------------------------------------------------------

bool PlayerUnitArmy::hasUnit(Unit* unit) {
	std::map<Ogre::String, Unit*>::iterator it = mUnitArmy.find(unit->getName());
	if (it != mUnitArmy.end()) {
		return true;
	}
	return false;
}
//----------------------------------------------------------------

std::map<Ogre::String, Unit*> PlayerUnitArmy::getUnitArmy() {
	return mUnitArmy;
}
//----------------------------------------------------------------