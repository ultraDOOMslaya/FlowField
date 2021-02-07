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

	void addTerrainValue(int x, int y, b2World* world, std::vector<GridSquare*>* impassableTerrain);
	virtual void addTerrainValue(Ogre::String squareName, Ogre::MaterialPtr	mat, b2World* world, std::vector<GridSquare*>* impassableTerrain);
	virtual void changeTileColor(int x, int y, Ogre::MaterialPtr mat, Ogre::ColourValue color);
	bool isImpassable(int x, int y);

	Ogre::SceneManager*						gameSceneManager;
	std::vector<std::vector<GridSquare*>>*  gridMap;
	
};

