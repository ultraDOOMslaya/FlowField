#pragma once
#ifndef __Soldier_h_
#define __Soldier_h_

#include <Ogre.h>

#include "Constants.h"
#include "PathFinding.h"
#include "SelectionCircle.h"
#include "GridUtils.h"

#include "Box2D\Box2D.h"

#include "Unit.h"

class Soldier : public Unit
{
private:


public:
	Soldier(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String name, Ogre::String meshName, Ogre::String unitClass, int ID, b2World* world);
	~Soldier();


};

#endif __Soldier_h_