#pragma once
#ifndef __Building_h_
#define __Building_h_

#include <Ogre.h>

#include "Constants.h"
#include "GridSquare.h"

#include "Box2D\Box2D.h"

class Building
{
public:
	Building(Ogre::SceneManager* gameSceneManager, Ogre::Vector3 startPos, Ogre::String name, Ogre::String meshName, b2World* world, std::vector<GridSquare*>* impassableTerrain);
	~Building();

	Ogre::SceneManager*					mScnMgr;
};

#endif __Building_h_


