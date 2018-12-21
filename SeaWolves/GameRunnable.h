#pragma once
#ifndef __GameRunnable_h_
#define __GameRunnable_h_

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreBitesConfigDialog.h>

#include <OgreOverlayManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElementFactory.h>

#include <OgreTrays.h>
#include <SDL.h>

#include "Constants.h"
#include "Bezier.h"
#include "SquareNeighbor.h"
#include "Unit.h"
#include "SteeringBehaviour.h"
#include "PathFinding.h"
#include "PlayerManager.h"
#include "SelectionCircle.h"
#include "SelectionBox.h"
#include "GridUtils.h"
#include "GridEditor.h"
#include "GenerateUnits.h"



class GameRunnable : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
	GameRunnable();
	~GameRunnable();

	void setup(void);
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
	virtual bool mouseMoved(const OgreBites::MouseMotionEvent &evt);
	virtual bool mousePressed(const OgreBites::MouseButtonEvent &evt);
	virtual bool mouseReleased(const OgreBites::MouseButtonEvent &evt);
	virtual void performSelection(const Ogre::Vector2& first, const Ogre::Vector2& second);
	virtual void getAllNeighbors(Unit unit);
	virtual void changeSquareColor(int num);

	virtual void frameRendered(const Ogre::FrameEvent& evt);
	virtual Ogre::Vector3 GameRunnable::staticObjectCollisionForceApplier(Unit* unit);

	virtual void createSquare(int width, int height, int edgeLength, std::string meshName, bool oddOrEven, Ogre::ColourValue color);
	virtual void createTileMap(void);

	OgreBites::TrayManager*		mTrayMgr;
	OgreBites::TextBox*			mCordPanel;			// Coordinates displayer
	OgreBites::CheckBox*		mModeCB;
	OgreBites::CheckBox*		mUnitCB;
	OgreBites::CheckBox*		mShowFlowPathCB;

	Ogre::SceneManager*			mScnMgr;
	Ogre::Camera*				mCam;
	Ogre::RenderWindow*         mWindow;
	Ogre::RaySceneQuery*		mRayScnQuery;
	Ogre::AnimationState*		robotAnimState;
	Ogre::SceneNode*			mUnitNode;
	Ogre::Real					mWalkSpeed;
	Ogre::Vector3				mDirection; //direction the object is moving
	Ogre::Real					mDistance; // distance object has left to travel
	Ogre::Vector3				mDestination; // destination the object is moving towards
	Ogre::Entity*				robotEntity;
	Ogre::SceneNode*			camNode;

	std::deque<Ogre::Vector3>   mWalkList;
	std::queue<SquareNeighbor*> squareNeighbors;
	std::queue<SquareNeighbor*> unitNeighbors;
	Ogre::Vector2				currentPos;
	
	bool						createUnitMode;
	//std::vector<Unit>			units;
	std::map<Ogre::String, Unit> units;
	std::queue<Ogre::String>	robots;
	int							robotNumber;
	int							debugx;
	int							debugy;
	int							debugz;
	int							pathLines;
	Ogre::MaterialPtr			mat;

	std::vector<Ogre::Vector2>	impassableTerrain;
	std::vector<PlayerManager>	players;
	PlayerManager				player1;
	GridEditor*					gridEditor;
	SelectionBox*				selectBox;
	Ogre::PlaneBoundedVolumeListSceneQuery* volQuery;
};

#endif __GameRunnable_h_