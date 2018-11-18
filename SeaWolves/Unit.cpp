#include "Unit.h"


/* msScnmgr - scene reference used to manage this object
* startPos - starting coordinates in raw floats for this unit
* BradsBitch - unique name identifier for this unit and a reference to the great Brad Gerhke
*/
Unit::Unit(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String BradsBitch)
	: direction(0, 0, 0),
	distance(0),
	destination(0, 0, 0),
	//walkSpeed(35),
	walkSpeed(2),
	minSeperation(35),
	maxCohesion(65),
	seperationRadius(0),							// lower the radius, greater the seperation 
	physicsBodyRadius(7),
	velocity(0, 0, 0),
	forceToApply(0, 0, 0),
	//maxForce(55),
	maxForce(3),
	//maxSpeed(35)
	maxSpeed(2),
	path(NULL)
{
	unitEntity = mScnMgr->createEntity("robot.mesh");
	unitEntity->setCastShadows(true);
	unitNode = mScnMgr->getRootSceneNode()->createChildSceneNode(BradsBitch, startPos);
	unitNode->attachObject(unitEntity);
	unitAnimState = unitEntity->getAnimationState("Idle");
	unitAnimState->setLoop(true);
	unitAnimState->setEnabled(true);
	unitName = BradsBitch;
	seperationRadius = physicsBodyRadius * 2;
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

Ogre::Vector3 Unit::getPosition() {
	return unitNode->getPosition();
}
//----------------------------------------------------------------

std::vector<std::vector<Ogre::Vector2>>* Unit::getFlowField() {
	return &path->flowField;
}
//----------------------------------------------------------------

Ogre::Vector2* Unit::getCurrentFlowValue() {
	return &path->flowField[currentPos.x][currentPos.y];
}
//----------------------------------------------------------------

bool Unit::hasLos() {
	return path->losGrid[currentPos.x][currentPos.y];
}
//----------------------------------------------------------------

void Unit::rotate(Ogre::Vector3 mDirection) {
	Ogre::Vector3 src = unitNode->getOrientation() * Ogre::Vector3::UNIT_X;

	/* Unit rotation code */
	/*if ((1.0 + src.dotProduct(mDirection)) < 0.0001) {
		unitNode->yaw(Ogre::Degree(180));
	}
	else {
		Ogre::Quaternion quat = src.getRotationTo(mDirection);
		unitNode->rotate(quat);
	}*/
	Ogre::Quaternion quat = src.getRotationTo(mDirection);
	unitNode->rotate(quat);
}
//----------------------------------------------------------------