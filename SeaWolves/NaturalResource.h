#pragma once
#ifndef __NaturalResource_h_
#define __NaturalResource_h_

#include <Ogre.h>

#include "Constants.h"
#include "PathFinding.h"
#include "GridUtils.h"
#include "GridEditor.h"

#include "Box2D\Box2D.h"

class NaturalResource
{
public:
	NaturalResource(Ogre::SceneManager* gameSceneManager, Ogre::Vector3 startPos, Ogre::String name, Ogre::String meshName, b2World* world, std::vector<GridSquare*>* impassableTerrain);
	~NaturalResource();

	Ogre::Vector3 getPosition();
	b2Vec2 getB2DPosition();

	Ogre::SceneManager*			mScnMgr;
	Ogre::SceneNode*			mNaturalResourceNode;
	Ogre::String				mName;
	Ogre::Vector2				mSpawnCoords;

	//Box2D Physics
	b2BodyDef					mBodyDef;
	b2Body*						mBody;
	b2World*					mWorld;
	bool						isSelected;
};

#endif __NaturalResource_h_