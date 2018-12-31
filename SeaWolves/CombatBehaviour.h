#pragma once

#include "PlayerManager.h"
#include "Unit.h"
#include "PlayerUtils.h"

class CombatBehaviour
{
public:
	static void huntForTarget(std::map<Ogre::String, Unit*>* units, PlayerManager* activePlayer, std::vector<PlayerManager*> players, Unit* unit);
	static void seekTarget(std::map<Ogre::String, Unit*>* units, PlayerManager* activePlayer, std::vector<PlayerManager*> players, Unit* unit);
};

