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

class Unit;

class MagicAttack
{
public:
	MagicAttack(Unit* unit, Ogre::SceneManager* mScnMgr);
	~MagicAttack();

	void dealDamage();
	void clearTarget();

	Ogre::SceneManager*				SceneManager;
	Ogre::String					particleName;
	Ogre::SceneNode*				particleNode;

	Unit*							summoner;

	Unit*						mTarget;
	int							mDamage;
	int							mTargetId;
};

#endif __MagicAttack_h_