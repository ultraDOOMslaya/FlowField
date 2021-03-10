#pragma once
#ifndef __GridSquare_h_
#define __GridSquare_h_

#include "Ogre.h"

#include "GridUtils.h"
#include "Constants.h"

#include "Box2D\Box2D.h"


class GridSquare
{
public:
	GridSquare(b2World* world, int x, int y);
	GridSquare(int x, int y, Ogre::String name, Ogre::ColourValue color, Ogre::MaterialPtr defaultMaterial);
	GridSquare(int x, int y);
	~GridSquare();

	virtual void defaultColor(Ogre::SceneManager* gameSceneManager);

	Ogre::Vector2				terrain;
	int							x;
	int							y;
	Ogre::String				entityName;
	Ogre::ColourValue			tileColor;
	Ogre::MaterialPtr			mDefaultMaterial;

	//Box2D Physics
	b2BodyDef					bodyDef;
	b2Body*						mBody;
};

#endif

