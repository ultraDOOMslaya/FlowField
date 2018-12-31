#include "PlayerUtils.h"

/*
 Cycle through every player for a given unit and determine its status given the current active player
*/
PlayerRelationshipStatus PlayerUtils::determineStatus(PlayerManager* activePlayer, std::vector<PlayerManager*> players, Unit* unit) {
	if (activePlayer->hasUnitInArmy(unit->unitName)) {
		return PlayerRelationshipStatus::ME;
	}
	else {
		for (std::vector<PlayerManager*>::iterator it = players.begin(); it < players.end(); it++) {
			PlayerManager* player = *it;
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
