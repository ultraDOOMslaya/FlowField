#pragma once
#ifndef __GameManager_h_
#define __GameManager_h_

#include "Ogre.h"

/** Other Managers **/


/** Things being managed **/
#include "Unit.h"
#include "Player.h"
#include "CombatBehaviour.h"



class GameManager
{
public:
	GameManager();
	~GameManager();

private:
	std::vector<Player*>		players;
	Player*						activePlayer;
	Player						player1;
	Player						player2;
};

#endif __GameManager_h_