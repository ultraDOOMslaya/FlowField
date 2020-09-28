#pragma once

#include "PlayerRelationshipStatus.h"
#include "Player.h"

class PlayerUtils
{
public:

	static PlayerRelationshipStatus determineStatus(Player* activePlayer, std::vector<Player*> players, Unit* unit);
};

