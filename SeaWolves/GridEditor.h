#pragma once

#include "Ogre.h"

#include "Constants.h"
#include "GridUtils.h"
#include "GridSquare.h"

#include "Box2D\Box2D.h"

class GridEditor
{
public:
	GridEditor(Ogre::SceneManager* mScnMgr, std::vector<std::vector<GridSquare*>>* mGridMap);
	~GridEditor();

	//virtual void addTerrainValue(Ogre::String squareName, Ogre::MaterialPtr	mat, std::vector<Ogre::Vector2>* impassableTerrain);
	virtual void addTerrainValue(Ogre::String squareName, Ogre::MaterialPtr	mat, std::vector<GridSquare*>* impassableTerrain, b2World* world);
	virtual void changeTileColor(int x, int y, Ogre::MaterialPtr mat, Ogre::ColourValue color);

	Ogre::SceneManager*					gameSceneManager;
	std::vector<std::vector<GridSquare*>>* gridMap;
};

