#pragma once
#ifndef __Tree_h_
#define __Tree_h_

#include <Ogre.h>

#include "NaturalResource.h"

#include "Box2D\Box2D.h"

class ForestTree : public NaturalResource
{
public:
	ForestTree(Ogre::SceneManager* gameSceneManager, Ogre::Vector3 startPos, Ogre::String name, Ogre::String meshName, b2World* world, std::vector<GridSquare*>* impassableTerrain);
	~ForestTree();

	int				mHarvestValue;
};

#endif __Tree_h_
