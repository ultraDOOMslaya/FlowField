#pragma once
#ifndef __Villager_h_
#define __Villager_h_

#include <Ogre.h>
#include <OgreTagPoint.h>

#include "Constants.h"
#include "PathFinding.h"
#include "SelectionCircle.h"
#include "GridUtils.h"
#include "GridEditor.h"

//#include "NaturalResource.h"

#include "Box2D\Box2D.h"

#include "Unit.h"

class Villager : public Unit
{
public:
	Villager(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String name, Ogre::String meshName, Ogre::String unitClass, int ID, b2World* world, std::vector<GridSquare*>* impassableTerrain, UnitController* unitController);
	~Villager();

	//void seekNaturalResource(NaturalResource* natResource);

	Ogre::Entity*				mWorkAxeEntity;
	Ogre::TagPoint*				mHandHoldPTagPoint;

	/** Villager unit states **/
	HuntingState*				mHuntingState;
};

#endif __Villager_h_