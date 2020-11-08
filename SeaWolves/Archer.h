#pragma once
#ifndef __Archer_h_
#define __Archer_h_

#include <Ogre.h>

#include "Constants.h"
#include "PathFinding.h"
#include "SelectionCircle.h"
#include "GridUtils.h"
#include "GridEditor.h"

#include "Box2D\Box2D.h"

#include "Unit.h"

class Archer : public Unit
{
public:
	Archer(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String BradsBitch, Ogre::String meshName, Ogre::String unitClass, int ID, b2World* world, std::vector<GridSquare*>* impassableTerrain, UnitController* unitController);
	~Archer();
};

#endif __Archer_h_