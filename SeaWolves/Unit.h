#pragma once
#ifndef __Unit_h_
#define __Unit_h_

#include <Ogre.h>
#include <OgreInput.h>
#include <OgreBitesConfigDialog.h>
#include "Constants.h"
#include "PathFinding.h"
#include "SelectionCircle.h"
#include "GridUtils.h"
#include "NaturalResource.h"

#include "UnitController.h"

//#include "UnitState.h"
#include "IdleState.h"
#include "WalkingState.h"
#include "AggressiveState.h"
#include "SeekingState.h"
#include "HuntingState.h"
#include "HarvestingState.h"
#include "AttackingState.h"

#include "Box2D\Box2D.h"

class PathFinding;  //Forward declaration
class UnitController;

//class UnitState;
class IdleState;
class WalkingState;
class AggressiveState;
class SeekingState;
class HuntingState;
class HarvestingState;
class AttackingState;

class Unit
{
public:
	enum State {
		STATE_IDLE,
		STATE_MARCHING,
		STATE_FINISH_JOURNEY,
		STATE_ATTACKING,
		STATE_SPAWN_SPELLACTION,
		STATE_HUNTING,
		STATE_SEEKING,
		STATE_AGGRESSIVE,
		STATE_POST_COMBAT,
		STATE_HARVESTING
	};

	Unit(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String name, Ogre::String meshName, Ogre::String unitClass, int ID, b2World* world, std::vector<GridSquare*>* impassableTerrain, UnitController* unitController);
	~Unit();
	virtual void commandMove(Ogre::Vector3 position);
	virtual void animate(Ogre::String animation);		//TODO make this animateLoop()
	virtual void animateSingle(Ogre::String animation);		//TODO make this animate()
	virtual bool isAnimation(Ogre::String animation);
	virtual void rotate(Ogre::Vector3 mDirection);
	virtual void rotate(b2Vec2 direction);
	virtual void haltTheGroup();
	virtual bool clearToMove();
	virtual bool groupHasLos();
	virtual bool assignedPathLosDiscovered();
	virtual void halt();
	virtual bool attackingTarget();
	virtual bool harvestingTarget();
	virtual bool isHunting();
	virtual bool isAttacking();
	virtual bool isAggressive();
	virtual bool isAtEndOfAnimation();
	virtual bool hasTarget();
	virtual bool hasPath();
	virtual void setTarget(Unit* target);
	virtual void setNatResourceTarget(NaturalResource* target);
	virtual void attack();
	virtual void harvest();
	virtual void doDamage();
	virtual void takeDamage(int damage);

	virtual Ogre::Vector3 getPosition();
	virtual b2Vec2 getB2DPosition();
	virtual std::vector<std::vector<Ogre::Vector2>>* getFlowField();
	virtual Ogre::Vector2* getCurrentFlowValue();
	virtual bool hasLos();
	virtual bool losTo(Unit* unit);
	virtual bool hasArrived();
	virtual bool nextLocation(void);
	virtual void selected(void);
	virtual void unselected(void);
	virtual bool inRange(Ogre::Vector3 targetPosition, int maxRange);
	virtual void setLooseTarget(Unit* unit, int distance);
	virtual void setHardTarget(void);
	virtual void seekTarget(std::map<Ogre::String, Unit*>* units);
	virtual void resetTarget();

	/** Utils **/
	virtual Ogre::Vector3 unitGroupConglomerate();
	virtual b2Vec2 seek(b2Vec2 destination);

	/** Handle inputs given to the unit **/
	virtual void handleInput(OgreBites::Event &evt);
	virtual void update(const Ogre::FrameEvent& evt);

	Ogre::SceneManager*			gameSceneManager;
	Ogre::AnimationState*		unitAnimState;
	Ogre::Animation*			unitAnimation;
	Ogre::SceneNode*			unitNode;
	Ogre::SceneNode*			projectileNode;
	Ogre::Entity*				unitEntity;
	Ogre::Entity*				projectileEntity;
	Ogre::String				unitName;
	Ogre::String				mUnitClass;

	Ogre::Vector3				direction; //direction the object is moving
	b2Vec2						b2Direction;
	Ogre::Real					distance; // distance object has left to travel
	float32						b2Distance;
	Ogre::Vector3				destination; // destination the object is moving towards
	b2Vec2						b2Destination;
	b2Vec2						postCombatB2Desination = b2Vec2_zero;
	Ogre::Vector3				finalDestination;
	b2Vec2						b2FinalDestination;

	Ogre::Vector2				currentPos;
	Ogre::Vector2				realizedPosition;
	Ogre::Vector2				debugPos1;
	Ogre::Vector2				debugPos2;
	b2Vec2						debugB2Pos1;
	int							debugInt1;
	Ogre::Real					animationElapsedTime = 1.0;
	std::deque<Ogre::Vector3>   walkList;
	std::deque<b2Vec2>			b2WalkList;

	int							minSeperation;
	int							maxCohesion;
	int							seperationRadius;
	int							physicsBodyRadius;
	int							unitID;
	int							attackRange;
	int							harvestRange;
	int							mHitPoints;
	int							mAttackDamage;
	int							distanceFromTarget;		//This is not continually updated and its state needs to be reset. Used while picking a target
	int							targetRadius;
	int							debugDump;
	int							projectileCount = 1000; //TODO this shouldn't be here
	bool						isSelected;
	bool						attacking;		//While attacking, will attack any target in an area
	bool						harvesting;
	bool						hunting;		//A move
	bool						trekking = false;			//Command move
	bool						hasAttacked = false;		//check to see if damage has been dealt over a given attack animation
	Unit*						mTarget;			//Target this unit is attacking
	NaturalResource*			mNatResourceTarget;	//Target this unit will harvest	
	Ogre::Real					maxForce;
	Ogre::Real					maxSpeed;
	Ogre::Real					walkSpeed;
	Ogre::Vector3				velocity;
	b2Vec2						b2Velocity;
	Ogre::Vector3				forceToApply;
	b2Vec2						b2ForceToApply;
	Ogre::Vector3				tempSeek;
	std::vector<Unit*>*			group;

	PathFinding*				path;
	SelectionCircle*			selectionCircle;
	UnitController*				mUnitController;

	//Box2D Physics
	b2BodyDef					bodyDef;
	b2Body*						mBody;
	b2World*					mWorld;

	State						mState;
	State						mPreviousState;
	//UnitState*				mUnitState;
	IdleState*					mIdleState;
	WalkingState*				mWalkingState;
	AggressiveState*			mAggressiveState;
	SeekingState*				mSeekingState;
	HuntingState*				mHuntingState;
	HarvestingState*			mHarvestingState;
	AttackingState*				mAttackingState;
	int							mPlayerId;

};

#endif __Unit_h_