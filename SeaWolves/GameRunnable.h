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



class GameRunnable : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
	GameRunnable();
	~GameRunnable();

	void setup(void);
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
	virtual bool mouseMoved(const OgreBites::MouseMotionEvent &evt);
	virtual bool mousePressed(const OgreBites::MouseButtonEvent &evt);
	virtual bool nextLocation(void);
	virtual void getNeighbors(SquareNeighbor point);
	virtual void getAllNeighbors(SquareNeighbor point);
	virtual void changeSquareColor(int num);

	virtual void frameRendered(const Ogre::FrameEvent& evt);
	virtual Ogre::Vector2 gridSquareCordFinder(std::string squareName);
	virtual Ogre::Vector2 numericalCordFinder(Ogre::Vector2 cordinates);
	virtual Ogre::Vector2 gridIndexFinder(Ogre::String squareName);
	virtual void generateDijkastraGrid2(Ogre::Vector2 point);
	virtual void steeringBehaviourFlowField(Ogre::Vector2 position);
	virtual void findPath(Ogre::Vector2 origin, Unit unit);
	virtual Ogre::Vector3* interpolateMovement(Ogre::Vector2 start, Ogre::Vector2 end);

	virtual void createSquare(int width, int height, int edgeLength, std::string meshName, bool oddOrEven, Ogre::ColourValue color);
	virtual void createTileMap(void);
	virtual void generateFlowField(void);

	OgreBites::TrayManager*		mTrayMgr;
	OgreBites::TextBox*			mCordPanel;			// Coordinates displayer

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
	Ogre::Vector2				currentPos;

	std::vector<std::vector<int>>				dijkastraGrid2d;
	std::vector<std::vector<Ogre::Vector2>>		flowField;
	bool										flowFieldLock;
	
	bool						createUnitMode;
	std::vector<Unit>			units;
	std::queue<Ogre::String>	robots;
	int							robotNumber;

	Constants*					CONSTANTS;
	Bezier*						Bezier;



};

#endif __GameRunnable_h_