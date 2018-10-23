#include "Unit.h"


/* msScnmgr - scene reference used to manage this object
* startPos - starting coordinates in raw floats for this unit
* BradsBitch - unique name identifier for this unit and a reference to the great Brad Gerhke
*/
Unit::Unit(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String BradsBitch)
	: direction(0, 0, 0),
	distance(0),
	destination(0, 0, 0),
	walkSpeed(35)
{
	unitEntity = mScnMgr->createEntity("robot.mesh");
	unitEntity->setCastShadows(true);
	unitNode = mScnMgr->getRootSceneNode()->createChildSceneNode(BradsBitch, startPos);
	unitNode->attachObject(unitEntity);
	unitAnimState = unitEntity->getAnimationState("Idle");
	unitAnimState->setLoop(true);
	unitAnimState->setEnabled(true);
}


Unit::~Unit()
{
}


void Unit::animate(Ogre::String animation) {
	unitAnimState = unitEntity->getAnimationState(animation);
	unitAnimState->setLoop(true);
	unitAnimState->setEnabled(true);
}
//----------------------------------------------------------------

bool Unit::nextLocation()
{
	if (walkList.empty()) { return false; }
	destination = walkList.front();  // this gets the front of the deque
	walkList.pop_front();             // this removes the front of the deque
	direction = destination - unitNode->getPosition();
	distance = direction.normalise();
	return true;
}
//----------------------------------------------------------------