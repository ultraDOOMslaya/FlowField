#pragma once
#ifndef __Wizard_h_
#define __Wizard_h_

#include <Ogre.h>

#include "Constants.h"
#include "PathFinding.h"
#include "SelectionCircle.h"
#include "GridUtils.h"
#include "GridEditor.h"

#include "Box2D\Box2D.h"

#include "Unit.h"

class Wizard : public Unit
{
public:
	Wizard(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String name, Ogre::String meshName, Ogre::String unitClass, int ID, b2World* world, std::vector<GridSquare*>* impassableTerrain);
	~Wizard();
};

#endif __Wizard_h_