#include "Unit.h"


/* mScnmgr - scene reference used to manage this object
* startPos - starting coordinates in raw floats for this unit
* BradsBitch - unique name identifier for this unit and a reference to the great Brad Gerhke
*/
Unit::Unit(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String BradsBitch, Ogre::String meshName, int ID)
	: direction(0, 0, 0),
	distance(0),
	destination(0, 0, 0),
	//walkSpeed(15),
	walkSpeed(35),								// delta time
	//walkSpeed(5),								// mocked time
	minSeperation(40),
	maxCohesion(100),
	seperationRadius(0),							// lower the radius, greater the seperation 
	physicsBodyRadius(7),
	velocity(0, 0, 0),
	forceToApply(0, 0, 0),
	maxForce(700),									// delta time (This needs to be x10 maxSpeed otherwise turns aren't very crisp)
	//maxForce(15),									// mocked time
	//maxSpeed(35),
	maxSpeed(50),									// delta time
	//maxSpeed(5),									// mocked time
	path(NULL),
	isSelected(false),
	attacking(false),
	mTarget(NULL),
	attackRange(0),
	distanceFromTarget(0),
	targetRadius(0)
{
	//unitEntity = mScnMgr->createEntity("robot.mesh");
	gameSceneManager = mScnMgr;
	unitEntity = gameSceneManager->createEntity(meshName);
	unitEntity->setCastShadows(true);
	unitNode = gameSceneManager->getRootSceneNode()->createChildSceneNode(BradsBitch, startPos);
	unitNode->setScale(50, 50, 50);
	unitNode->attachObject(unitEntity);

	unitAnimState = unitEntity->getAnimationState("Idle");
	unitAnimState->setLoop(true);
	unitAnimState->setEnabled(true);

	unitName = BradsBitch;
	seperationRadius = physicsBodyRadius * 2;
	targetRadius = 500;
	unitEntity->setQueryFlags(Constants::unitQueryMask);
	unitID = ID;
}


Unit::~Unit()
{
}

/*
 Called every frame for moving units.
*/
void Unit::commandMove(Ogre::Vector3 position) {
	unitNode->setPosition(position);
	if (isSelected) {
		selectionCircle->move(position);
	}
}
//----------------------------------------------------------------

void Unit::animate(Ogre::String animation) {
	unitAnimState = unitEntity->getAnimationState(animation);
	unitAnimState->setLoop(true);
	unitAnimState->setEnabled(true);
}
//----------------------------------------------------------------

void Unit::haltTheGroup() {
	for (std::vector<Unit*>::iterator it = group->begin(); it != group->end(); it++) {
		int distance = GridUtils::distanceTo(getPosition(), (*it)->getPosition());

		if (distance < maxCohesion) {
			(*it)->halt();
		}
	}
}
//----------------------------------------------------------------

void Unit::halt() {
	destination = Ogre::Vector3::ZERO;
	forceToApply = Ogre::Vector3::ZERO;
	animate("Idle");
}
//----------------------------------------------------------------

bool Unit::attackingTarget() {
	if (mTarget) {
		finalDestination = mTarget->getPosition();
		if (inRange()) {
			attack();
		}
		return true;
	}
	return false;
}
//----------------------------------------------------------------

void Unit::setTarget(Unit* target) {
	mTarget = target;
	attacking = true;
}
//----------------------------------------------------------------

void Unit::attack() {
	halt();
	rotate((mTarget->getPosition() - getPosition()));
	animate("Attack");
}
//----------------------------------------------------------------

bool Unit::isAtEndOfAnimation() {
	return unitAnimState->hasEnded();
}
//----------------------------------------------------------------

void Unit::seekTarget(std::map<Ogre::String, Unit*>* units) {
	
}
//----------------------------------------------------------------

bool Unit::inRange() {
	int distance = GridUtils::distanceTo(mTarget->getPosition(), getPosition());
	if (distance < attackRange) {
		return true;
	}
	return false;
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

bool Unit::losTo(Unit* unit) {
	Ogre::Vector2 cords = GridUtils::cordNumericalFinder(unit->getPosition());
	return path->losGrid[cords.x][cords.y];
}
//----------------------------------------------------------------

bool Unit::hasArrived() {
	int x, z;
	Ogre::Vector3 distanceLeft = getPosition() - finalDestination;
	x = distanceLeft.x;
	z = distanceLeft.z;
	if ((x < 1 && x > -1) && (z < 1 && z > -1)) {
		halt();
		return true;
	}
	return false;
}
//----------------------------------------------------------------

void Unit::rotate(Ogre::Vector3 mDirection) {
	Ogre::Vector3 src = unitNode->getOrientation() * Ogre::Vector3::UNIT_X;

	/* Unit rotation code */
	if ((1.0 + src.dotProduct(mDirection)) < 0.0001) {
		unitNode->yaw(Ogre::Degree(180));
	}
	else {
		Ogre::Quaternion quat = src.getRotationTo(mDirection);
		unitNode->rotate(quat);
	}
	//Ogre::Quaternion quat = src.getRotationTo(mDirection);
	//unitNode->rotate(quat);
}
//----------------------------------------------------------------

void Unit::selected() {
	isSelected = true;
	selectionCircle = new SelectionCircle(gameSceneManager, getPosition(), unitID);
}
//----------------------------------------------------------------

void Unit::unselected() {
	isSelected = false;
	delete selectionCircle;
}
//----------------------------------------------------------------

void Unit::setLooseTarget(Unit* potentialTarget, int distance) {
	mTarget = potentialTarget;
	distanceFromTarget = distance;
	attacking = true;
}
//----------------------------------------------------------------

void Unit::setHardTarget() {

}
//----------------------------------------------------------------

bool Unit::isHunting() {
	//if (attacking && hunting) {
	if (hunting) {
		return true;
	}
	return false;
}
//----------------------------------------------------------------

bool Unit::isAttacking() {
	if (attacking) {
		return true;
	}
	return false;
}
//----------------------------------------------------------------

bool Unit::isAggressive() {
	if (isHunting() || isAttacking()) {
		return true;
	}
	return false;
}
//----------------------------------------------------------------

bool Unit::hasTarget() {
	if (mTarget) {
		return true;
	}
	return false;
}
//----------------------------------------------------------------
