#pragma once
#ifndef __Unit_h_
#define __Unit_h_

#include <Ogre.h>

#include "Constants.h"
#include "PathFinding.h"
#include "SelectionCircle.h"
#include "GridUtils.h"

#include "Box2D\Box2D.h"

class PathFinding;  //Forward declaration

class Unit
{
public:
	Unit(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String name, Ogre::String meshName, Ogre::String unitClass, int ID, b2World* world);
	~Unit();
	virtual void commandMove(Ogre::Vector3 position);
	virtual void animate(Ogre::String animation);
	virtual void rotate(Ogre::Vector3 mDirection);
	virtual void rotate(b2Vec2 direction);
	virtual void haltTheGroup();
	virtual bool clearToMove();
	virtual bool groupHasLos();
	virtual bool assignedPathLosDiscovered();
	virtual void halt();
	virtual bool attackingTarget();
	virtual bool isHunting();
	virtual bool isAttacking();
	virtual bool isAggressive();
	virtual bool isAtEndOfAnimation();
	virtual bool hasTarget();
	virtual void setTarget(Unit* target);
	virtual void attack();

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
	virtual bool inRange(void);
	virtual void setLooseTarget(Unit* unit, int distance);
	virtual void setHardTarget(void);
	virtual void seekTarget(std::map<Ogre::String, Unit*>* units);

	Ogre::SceneManager*			gameSceneManager;
	Ogre::AnimationState*		unitAnimState;
	Ogre::SceneNode*			unitNode;
	Ogre::Entity*				unitEntity;
	Ogre::String				unitName;
	Ogre::String				mUnitClass;

	Ogre::Vector3				direction; //direction the object is moving
	b2Vec2						b2Direction;
	Ogre::Real					distance; // distance object has left to travel
	float32						b2Distance;
	Ogre::Vector3				destination; // destination the object is moving towards
	b2Vec2						b2Destination;
	Ogre::Vector3				finalDestination;
	b2Vec2						b2FinalDestination;

	Ogre::Vector2				currentPos;
	Ogre::Vector2				realizedPosition;
	Ogre::Vector2				debugPos1;
	Ogre::Vector2				debugPos2;
	b2Vec2						debugB2Pos1;
	int							debugInt1;
	std::deque<Ogre::Vector3>   walkList;
	std::deque<b2Vec2>			b2WalkList;

	int							minSeperation;
	int							maxCohesion;
	int							seperationRadius;
	int							physicsBodyRadius;
	int							unitID;
	int							attackRange;
	int							distanceFromTarget;		//This is not continually updated and its state needs to be reset. Used while picking a target
	int							targetRadius;
	int							debugDump;
	bool						isSelected;
	bool						attacking;		//While attacking, will attack any target in an area
	bool						hunting;
	Unit*						mTarget;			//Target this unit is attacking
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

	//Box2D Physics
	b2BodyDef					bodyDef;
	b2Body*						mBody;

};

#endif __Unit_h_