#include "PlayerUtils.h"

/*
 Cycle through every player for a given unit and determine its status given the current active player
*/
PlayerRelationshipStatus PlayerUtils::determineStatus(Player* activePlayer, std::vector<Player*> players, Unit* unit) {
	if (activePlayer->hasUnitInArmy(unit->unitName)) {
		return PlayerRelationshipStatus::ME;
	}
	else {
		for (std::vector<Player*>::iterator it = players.begin(); it < players.end(); it++) {
			Player* player = *it;
			if (player->hasUnitInArmy(unit->unitName)) {
				if (player->relationship->isFoe()) {
					return PlayerRelationshipStatus::HOSTILE;
				}
			}
		}
	}
	return PlayerRelationshipStatus::FRIENDLY;
}
//----------------------------------------------------------------
