#pragma once
#ifndef __MapManager_h_
#define __MapManager_h_

#include <Ogre.h>

//Map & Grid
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include "Box2D\Box2D.h"
#include "GridSquare.h"
#include "GridEditor.h"

#include "Constants.h"

class MapManager
{
public:
	MapManager(Ogre::SceneManager* sceneManager, Ogre::Light* light, std::vector<GridSquare*>* impassableTerrain, b2World* world, GridEditor* gridEditor);
	~MapManager();

	void configureTerrainDefaults(Ogre::Light* light);
	void defineTerrain(long x, long y);
	void initBlendMaps(Ogre::Terrain* terrain);

	Ogre::TerrainGroup::RayResult getRayResult(Ogre::Ray* ray);

	Ogre::SceneManager*				mScnMgr;

	bool							mTerrainsImported;
	Ogre::TerrainGroup*				mTerrainGroup;
	Ogre::TerrainGlobalOptions*		mTerrainGlobals;
	std::vector<GridSquare*>*		mImpassableTerrain;
	b2World*						mWorld;
	GridEditor*						mGridEditor;

};

#endif __MapManager_h_