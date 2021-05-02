#pragma once
#ifndef __Barracks_h_
#define __Barracks_h_

#include <Ogre.h>

#include "Building.h"
#include "Constants.h"
#include "GridSquare.h"
#include "GridEditor.h"
#include "GridUtils.h"
#include "SelectionCircle.h"

#include "Box2D\Box2D.h"

class Barracks : public Building
{
public:
	Barracks(Ogre::SceneManager* gameSceneManager, Ogre::Vector3 buildingPos, int buildingId, Ogre::String meshName, b2World* world, std::vector<GridSquare*>* impassableTerrain, int ticksToCompletion);
	~Barracks();

	Ogre::Vector2		mBuildingSpace[9];
	Ogre::String		mMeshName;
};

#endif __Barracks_h_