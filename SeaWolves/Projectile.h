#pragma once
#ifndef __Projectile_h_
#define __Projectile_h_

#include <Ogre.h>

#include "Constants.h"
#include "PathFinding.h"
#include "SelectionCircle.h"
#include "GridUtils.h"

#include "Box2D\Box2D.h"

#include "Unit.h"

class Unit;

class Projectile
{
public:
	Projectile(Unit* unit, Ogre::SceneManager* mScnMgr);
	~Projectile();

	void dealDamage();
	void clearTarget();
	void halt();

	Ogre::SceneManager*			SceneManager;
	Ogre::Entity*				projectileEntity;
	Ogre::String				projectileName;
	Ogre::SceneNode*			projectileNode;

	Ogre::Real					mWalkSpeed = 210.0;
	Ogre::Real					mDistance;
	Ogre::Vector3				mDirection;
	Ogre::Vector3				mDestination;

	b2Vec2						b2Destination;
	
	Ogre::Vector3				finalDestination;
	b2Vec2						b2FinalDestination;
	Unit*						mTarget;
	Unit*						mFletcher;
	int							mDamage;
	int							mTargetId;
};

#endif __Projectile_h_