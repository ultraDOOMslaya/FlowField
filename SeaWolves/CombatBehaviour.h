#pragma once

#include "Player.h"
#include "Unit.h"
#include "PlayerUtils.h"
#include "Projectile.h"
#include "MagicAttack.h"

class CombatBehaviour
{
public:
	static void huntForTarget(std::map<Ogre::String, Unit*>* units, Player* activePlayer, std::vector<Player*> players, Unit* unit);
	static void seekTarget(std::map<Ogre::String, Unit*>* units, Player* activePlayer, std::vector<Player*> players, Unit* unit);
	static void clearTargets(std::map<Ogre::String, Unit*>* units, Unit* expiredTarget);
	static void spawnSpellAction(Unit* unit, std::vector<Projectile*>* projectiles, std::vector<MagicAttack*>* magicAttacks, Ogre::SceneManager* sceneMgr);
};

