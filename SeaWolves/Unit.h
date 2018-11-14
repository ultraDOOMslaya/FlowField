#pragma once
#ifndef __Unit_h_
#define __Unit_h_

#include <Ogre.h>
#include "Constants.h"

class Unit
{
public:
	Unit(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String name);
	~Unit();
	virtual void animate(Ogre::String animation);
	virtual void rotate(Ogre::Vector3 mDirection);
	virtual Ogre::Vector3 getPosition();
	virtual bool nextLocation(void);

	Ogre::AnimationState*		unitAnimState;
	Ogre::SceneNode*			unitNode;
	Ogre::Entity*				unitEntity;
	Ogre::String				unitName;

	Ogre::Vector3				direction; //direction the object is moving
	Ogre::Real					distance; // distance object has left to travel
	Ogre::Vector3				destination; // destination the object is moving towards
	Ogre::Vector3				finalDestination;

	Ogre::Vector2				currentPos;
	Ogre::Vector2				realizedPosition;
	std::deque<Ogre::Vector3>   walkList;

	int							minSeperation;
	int							maxCohesion;
	int							radius;
	Ogre::Real					maxForce;
	Ogre::Real					maxSpeed;
	Ogre::Real					walkSpeed;
	Ogre::Vector3				velocity;
	Ogre::Vector3				forceToApply;
	Ogre::Vector3				tempSeek;

};

#endif __Unit_h_