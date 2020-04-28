#include "Unit.h"


/* mScnmgr - scene reference used to manage this object
* startPos - starting coordinates in raw floats for this unit
* BradsBitch - unique name identifier for this unit and a reference to the great Brad Gerhke
*/
Unit::Unit(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String BradsBitch, Ogre::String meshName, Ogre::String unitClass, int ID, b2World* world)
	: direction(0, 0, 0),
	distance(0),
	destination(0, 0, 0),
	//walkSpeed(15),
	walkSpeed(35),								// delta time
	//walkSpeed(5),								// mocked time
	minSeperation(40),
	maxCohesion(300),
	seperationRadius(0),							// lower the radius, greater the seperation 
	physicsBodyRadius(6),
	velocity(0, 0, 0),
	b2Velocity(0, 0),
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
	gameSceneManager = mScnMgr;
	unitEntity = gameSceneManager->createEntity(meshName);
	unitEntity->setCastShadows(true);
	unitNode = gameSceneManager->getRootSceneNode()->createChildSceneNode(BradsBitch, startPos);
	unitNode->setScale(50, 50, 50);
	unitNode->attachObject(unitEntity);
	mUnitClass = unitClass;

	unitAnimState = unitEntity->getAnimationState("Idle");
	unitAnimState->setLoop(true);
	unitAnimState->setEnabled(true);

	unitName = BradsBitch;
	seperationRadius = physicsBodyRadius * 2;
	targetRadius = 500;
	unitEntity->setQueryFlags(Constants::unitQueryMask);
	unitID = ID;

	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(startPos.x, startPos.z);
	mBody = world->CreateBody(&bodyDef);

	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0); //position, relative to body position
	circleShape.m_radius = (seperationRadius); //radius - around 7 half the radius of the actual graphic

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.4f;
	fixtureDef.restitution = 0.2f;
	mBody->CreateFixture(&fixtureDef);
}


Unit::~Unit()
{
}

/*
 Called every frame for moving units.
*/
void Unit::commandMove(Ogre::Vector3 position) {
	unitNode->setPosition(position);
	//b2Vec2 newPos(position.x, position.z);
	//mBody->SetTransform(newPos, mBody->GetAngle());
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

bool Unit::groupHasLos() {
	if (group != NULL) {
		for (std::vector<Unit*>::iterator it = group->begin(); it != group->end(); it++) {
			if ((*it)->hasLos()) {
				return false;
			}
		}
	}

	return true;
}
//----------------------------------------------------------------

/*
* Group behaviour around clearPoints
*/
bool Unit::clearToMove() {
	if (group != NULL) {
		for (std::vector<Unit*>::iterator it = group->begin(); it != group->end(); it++) {
			if ((*it)->mUnitClass == "HeavyArmor") {
				int distance = (*it)->getPosition().squaredDistance(Ogre::Vector3((*it)->b2FinalDestination.x, 0, (*it)->b2FinalDestination.y));
				if (path != NULL && (distance > path->clearPointDistance)) {
					return false;
				}
			}
		}
	}
	
	return true;
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
	b2Destination = b2Vec2_zero;
	destination = Ogre::Vector3::ZERO;
	forceToApply = Ogre::Vector3::ZERO;
	mBody->SetLinearVelocity(b2Vec2(0, 0));
 	mBody->SetAngularVelocity(0);
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
	if (b2WalkList.empty()) { return false; }
	b2Destination = b2WalkList.front();
	b2WalkList.pop_front();
	b2Direction = b2Destination - getB2DPosition();
	b2Distance = b2Direction.Normalize();
	return true;
}
//----------------------------------------------------------------

Ogre::Vector3 Unit::getPosition() {
	return unitNode->getPosition();
}
//----------------------------------------------------------------

b2Vec2 Unit::getB2DPosition() {
	return mBody->GetPosition();
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
	int x, y;
	b2Vec2 distanceLeft = getB2DPosition() - b2FinalDestination;
	x = distanceLeft.x;
	y = distanceLeft.y;
	if ((x < 1 && x > -1) && (y < 1 && y > -1)) {
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

void Unit::rotate(b2Vec2 direction) {
	Ogre::Vector3 src = unitNode->getOrientation() * Ogre::Vector3::UNIT_X;
	Ogre::Vector3 graphicDirection = Ogre::Vector3(direction.x, 0, direction.y);
	/* Unit rotation code */
	if ((1.0 + src.dotProduct(graphicDirection)) < 0.0001) {
		unitNode->yaw(Ogre::Degree(180));
	}
	else {
		Ogre::Quaternion quat = src.getRotationTo(graphicDirection);
		unitNode->rotate(quat);
	}
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
