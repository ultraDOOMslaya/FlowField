#pragma once
#ifndef __PlayerInputManager_h_
#define __PlayerInputManager_h_

#include "Ogre.h"

#include "GridSquare.h"
#include "PlayerManager.h"

class PlayerInputManager
{
private:
	Ogre::Camera* camera;
	Ogre::PlaneBoundedVolumeListSceneQuery* volumeQuery;
	Ogre::SceneManager* sceneManager;

public:
	PlayerInputManager(Ogre::Camera* mCam, Ogre::PlaneBoundedVolumeListSceneQuery* mVolQuery, Ogre::SceneManager* mScnMgr);
	~PlayerInputManager();

	void performSelection(const Ogre::Vector2& first, const Ogre::Vector2& second, PlayerManager* activePlayer, std::vector<std::vector<GridSquare*>> &gridMap);
	void clearFocusedLocations(PlayerManager* activePlayer, std::vector<std::vector<GridSquare*>> &gridMap);
};

#endif