#pragma once
#ifndef __GameRunnable_h_
#define __GameRunnable_h_

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreBitesConfigDialog.h>
#include <OgreTagPoint.h>

#include <OgreOverlayManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElementFactory.h>

#include <OgreTrays.h>
#include <SDL.h>

#include "Constants.h"
#include "SquareNeighbor.h"
#include "SteeringBehaviour.h"
#include "PathFinding.h"
#include "Player.h"
#include "SelectionCircle.h"
#include "SelectionBox.h"
#include "GridUtils.h"
#include "GridEditor.h"
#include "GridSquare.h"
#include "GenerateUnits.h"
#include "PlayerRelationship.h"
#include "PlayerRelationshipStatus.h"
#include "PlayerUtils.h"
#include "CombatBehaviour.h"
#include "AIUtils.h"

//Manager Singletons
#include "PlayerInputManager.h"
#include "GameManager.h"
#include "UnitController.h"

//Net Code
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"  // MessageID

//GameObjects
#include "Unit.h"
#include "MagicAttack.h"
#include "Building.h"
#include "ForestTree.h"
#include "Villager.h"

//Map & Grid
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include "MapManager.h"
#include "Box2D\Box2D.h"

struct Cell {
	int x;
	int y;
	int unpassable;
};

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
	ID_GAME_UNIT_VELOCITY = ID_USER_PACKET_ENUM + 2
};

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
	virtual void changeSquareColor(int num);
	virtual void frameRendered(const Ogre::FrameEvent& evt);


	virtual void createTileMap(void);


	virtual void spawnProjectile(Unit* unit);
	virtual void spawnMagic(Unit* unit);
	virtual void clearTargets(Unit* unit);
	bool doesUnitExist(int unitId);

	#define SERVER_PORT 60000

	OgreBites::TrayManager*		mTrayMgr;
	OgreBites::TextBox*			mCordPanel;			// Coordinates displayer
	OgreBites::CheckBox*		mModeCB;
	OgreBites::CheckBox*		mUnitCB;
	OgreBites::CheckBox*		mShowFlowPathCB;
	OgreBites::CheckBox*		mNonCombat;
	OgreBites::CheckBox*		mMoveableCamera;

	//Top left panel
	OgreBites::CheckBox*		mAlterTextureCB;
	OgreBites::CheckBox*		mAlterElevationCB;
	OgreBites::SelectMenu*		mGroundTypeSM;
	OgreBites::Slider*			mElevationSlider;

	Ogre::Light*				spotLight;
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
	Ogre::SceneNode*			camAnchor;

	std::deque<Ogre::Vector3>   mWalkList;
	std::queue<SquareNeighbor*> squareNeighbors;
	std::queue<SquareNeighbor*> unitNeighbors;
	Ogre::Vector2				currentPos;
	
	bool						createUnitMode;
	//std::vector<Unit>			units;
	std::map<Ogre::String, Unit*> units;
	std::map<Ogre::String, NaturalResource*> natResources;

	std::vector<Projectile*>	projectiles;
	std::vector<MagicAttack*>	magicAttacks;
	std::queue<Ogre::String>	robots;
	int							robotNumber;
	int							debugx;
	int							debugy;
	int							debugz;
	int							pathLines;
	Ogre::MaterialPtr			mat;
	Ogre::MaterialPtr			mapTileMat1;
	Ogre::MaterialPtr			mapTileMat2;
	Ogre::MaterialPtr			greenMat;
	Ogre::MaterialPtr			redMat;

	std::vector<std::vector<GridSquare*>> gridMap; //TODO put this in a map class of some sort and consolidate the non static items in gridUtils into it.
	std::vector<Player*>	players;
	Player*				activePlayer;
	Player				player1;
	Player				player2;
	GridEditor*					gridEditor;
	std::vector<GridSquare*>    impassableTerrain;
	SelectionBox*				selectBox;
	Ogre::PlaneBoundedVolumeListSceneQuery* volQuery;

	//GameObjects
	Building*					mRedBuilding;
	Building*					mBlueBuilding;

	//Manager Singletons
	//TODO why are these pointers?
	UnitController*				mUnitController;
	PlayerInputManager*			pim;
	GameManager*				GameMgr;
	MapManager*					mMapManager;

	//Net Code
	RakNet::Packet*				packet;
	RakNet::RakPeerInterface*	peer;
	RakNet::SocketDescriptor	sd;

	//Box2d Physics
	b2World*					mWorld;	
};

#endif __GameRunnable_h_