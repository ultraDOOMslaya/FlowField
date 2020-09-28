#pragma once
#ifndef __PlayerInputManager_h_
#define __PlayerInputManager_h_

#include "Ogre.h"

#include "GridSquare.h"
#include "Player.h"
#include "SelectionBox.h"

class PlayerInputManager
{
private:
	Ogre::Camera* camera;
	Ogre::PlaneBoundedVolumeListSceneQuery* volumeQuery;
	Ogre::SceneManager* sceneManager;

public:
	PlayerInputManager(Ogre::Camera* mCam, Ogre::PlaneBoundedVolumeListSceneQuery* mVolQuery, Ogre::SceneManager* mScnMgr);
	~PlayerInputManager();

	void gameInputActionSelect_Release(SelectionBox* selectBox, Ogre::RaySceneQuery* raySceneQuery, Ogre::Ray& mouseRay);

	void performSelection(const Ogre::Vector2& first, const Ogre::Vector2& second, Player* activePlayer, std::vector<std::vector<GridSquare*>> &gridMap);
	void clearFocusedLocations(Player* activePlayer, std::vector<std::vector<GridSquare*>> &gridMap);
};

#endif