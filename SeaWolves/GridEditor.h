#pragma once

#include "Ogre.h"

#include "Constants.h"
#include "GridUtils.h"

class GridEditor
{
public:
	GridEditor(Ogre::SceneManager* mScnMgr);
	~GridEditor();

	virtual void addTerrainValue(Ogre::String squareName, Ogre::MaterialPtr	mat, std::vector<Ogre::Vector2>* impassableTerrain);

	Ogre::SceneManager*					gameSceneManager;
};

