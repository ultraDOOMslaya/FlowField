#pragma once
#ifndef __MagicAttack_h_
#define __MagicAttack_h_

#include <Ogre.h>

#include "Constants.h"
#include "PathFinding.h"
#include "SelectionCircle.h"
#include "GridUtils.h"

#include "Box2D\Box2D.h"

#include "Unit.h"


class MagicAttack
{
public:
	MagicAttack(Unit* unit, Ogre::SceneManager* mScnMgr, b2World* world);
	~MagicAttack();

	void dealDamage();

	Ogre::SceneManager*				SceneManager;
	Ogre::String					particleName;
	Ogre::SceneNode*				particleNode;

	Unit*							summoner;

	Unit*						mTarget;
	int							mDamage;
};

#endif __MagicAttack_h_