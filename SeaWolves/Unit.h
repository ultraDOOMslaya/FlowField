#pragma once
#ifndef __Unit_h_
#define __Unit_h_

#include <Ogre.h>

class Unit
{
public:
	Unit(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String name);
	~Unit();
	virtual void animate(Ogre::String animation);
	virtual bool nextLocation(void);

	Ogre::AnimationState*		unitAnimState;
	Ogre::SceneNode*			unitNode;
	Ogre::Entity*				unitEntity;
	Ogre::String				unitName;

	Ogre::Vector3				direction; //direction the object is moving
	Ogre::Real					distance; // distance object has left to travel
	Ogre::Vector3				destination; // destination the object is moving towards
	Ogre::Vector2				finalDestination;

	Ogre::Vector2				currentPos;
	std::deque<Ogre::Vector3>   walkList;
	Ogre::Real					walkSpeed;
};

#endif __Unit_h_