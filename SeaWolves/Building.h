#pragma once
#ifndef __Building_h_
#define __Building_h_

#include <Ogre.h>

#include "Constants.h"
#include "GridSquare.h"
#include "GridEditor.h"
#include "GridUtils.h"
#include "SelectionCircle.h"

#include "Box2D\Box2D.h"

class Building
{
public:
	Building(Ogre::SceneManager* gameSceneManager, Ogre::Vector3 startPos, Ogre::String name, Ogre::String meshName, b2World* world, std::vector<GridSquare*>* impassableTerrain);
	~Building();

	void addTerrainValue(int x, int y, b2World* world, std::vector<GridSquare*>* impassableTerrain);
	void selected();
	void unselected();

	Ogre::SceneManager*			mScnMgr;
	Ogre::SceneNode*			buildingNode;
	Ogre::String				mName;
	SelectionCircle*			mSelectionCircle;

	//Box2D Physics
	b2BodyDef					mBodyDef;
	b2Body*						mBody;
	b2World*					mWorld;
	bool						isSelected;
};

#endif __Building_h_


