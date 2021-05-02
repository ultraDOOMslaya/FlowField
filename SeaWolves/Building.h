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
	Building(Ogre::SceneManager* gameSceneManager, Ogre::Vector3 startPos, int buildingId, Ogre::String meshName, b2World* world, std::vector<GridSquare*>* impassableTerrain);
	~Building();

	void addTerrainValue(int x, int y, b2World* world, std::vector<GridSquare*>* impassableTerrain);
	void selected();
	void unselected();
	void setSpawnPoint(Ogre::Vector2 coords);
	Ogre::Vector2 getSpawnPoint();
	Ogre::Vector3 getPosition();
	b2Vec2 getB2DPosition();
	Ogre::String getName();

	Ogre::SceneManager*			mScnMgr;
	Ogre::Entity*				constructionEntity;
	Ogre::Entity*				buildingEntity;
	Ogre::SceneNode*			buildingNode;
	Ogre::String				mName;
	SelectionCircle*			mSelectionCircle;
	Ogre::Vector2				mSpawnCoords;


	//Box2D Physics
	b2BodyDef					mBodyDef;
	b2Body*						mBody;
	b2World*					mWorld;
	bool						isSelected;

	int							mBuildingId;
	int							mTicksToCompletion;
};

#endif __Building_h_


