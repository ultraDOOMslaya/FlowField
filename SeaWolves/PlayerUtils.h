#pragma once

#include "PlayerRelationshipStatus.h"
#include "PlayerManager.h"

class PlayerUtils
{
public:

	static PlayerRelationshipStatus determineStatus(PlayerManager* activePlayer, std::vector<PlayerManager*> players, Unit* unit);
};

