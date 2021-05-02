#pragma once

#include "Ogre.h"

class SelectionCircle
{
	Ogre::ManualObject*						circleXY;
	Ogre::SceneNode*						circleNode;
	Ogre::Vector3							startPosition;
	Ogre::SceneManager*						gameSceneManager;

public:
	SelectionCircle(Ogre::SceneManager* mScnMgr, Ogre::Vector3 position, int unitID, int gameObjectType, Ogre::Real circleRadius);
	SelectionCircle(Ogre::SceneManager* mScnMgr, Ogre::Vector3 position, int unitID, int gameObjectType);
	~SelectionCircle();
	virtual void move(Ogre::Vector3 position);

	
};

