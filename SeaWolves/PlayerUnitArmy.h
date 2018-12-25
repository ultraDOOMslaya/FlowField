#pragma once

#include "Ogre.h"
#include "Unit.h"

class PlayerUnitArmy
{
public:
	PlayerUnitArmy();
	PlayerUnitArmy(std::vector<Unit*> unitArmy);
	PlayerUnitArmy(std::vector<Unit> unitArmy);
	~PlayerUnitArmy();

	virtual void setUnitArmy(std::vector<Unit*> unitArmy);
	virtual bool hasUnit(Unit* unit);
	virtual std::map<Ogre::String, Unit*> getUnitArmy();

protected:
	std::map<Ogre::String, Unit*>						mUnitArmy;
};

