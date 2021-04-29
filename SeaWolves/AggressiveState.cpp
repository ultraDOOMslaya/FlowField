#include "AggressiveState.h"


void AggressiveState::enter(Unit& unit) {
	unit.mState = Unit::STATE_AGGRESSIVE;
	unit.animate("Walk");
}


void AggressiveState::handleInput(Unit& unit, OgreBites::Event& evt) {

}

void AggressiveState::update(Unit& unit, const Ogre::FrameEvent& evt) {
	if (unit.hasTarget()) {
		unit.distanceFromTarget = unit.getPosition().squaredLength() - unit.mTarget->getPosition().squaredLength();
		if (unit.inRange(unit.mTarget->getPosition(), unit.attackRange)) {
			unit.mState = Unit::STATE_ATTACKING;
			unit.mAttackingState->enter(unit);
		}
		else {
			unit.finalDestination = unit.mTarget->getPosition();
			unit.b2FinalDestination = unit.mTarget->getB2DPosition();
		}
	}
	else {
		unit.mUnitController->huntForTarget(&unit);
	}

	/* Realtime directing */
	Ogre::Vector2 position = GridUtils::b2CordNumericalFinder(unit.getB2DPosition());
	if (position != unit.currentPos) {
		unit.currentPos = position;
	}

	b2Vec2 cords;
	if (unit.hasLos()) {
		cords = unit.b2FinalDestination;
		if (unit.assignedPathLosDiscovered()) {
			unit.mUnitController->proximityLocationFormation(unit.path->origin.x, unit.path->origin.y, *(unit.group), unit.path);
		}
	}
	else {
		Ogre::Vector2 direction = GridUtils::numericalCordFinder(*(unit.getCurrentFlowValue()));
		cords = GridUtils::b2NumericalCordFinder(*(unit.getCurrentFlowValue()));
	}

	/* Physics applied to everyone */
	b2Vec2 seek = unit.seek(cords); //TODO this kinda sucks as seek should be a static method

	unit.b2ForceToApply = seek;

	if (unit.hasArrived()) {
		unit.mIdleState->enter(unit);
	}

	/** unit subroutine **/
	if (unit.nextLocation()) {
		unit.b2Velocity = unit.b2Direction;
		unit.mBody->SetType(b2_dynamicBody);
	}

	/** Marching and Hunting **/
	if (unit.b2Destination != b2Vec2_zero) {

		//unit.b2ForceToApply.operator*=(evt.timeSinceLastFrame);		//Real Time
		unit.b2ForceToApply.operator*=(0.0166666);			//Mocked Delta Time
		unit.b2Velocity.operator+=(unit.b2ForceToApply);

		float speed = unit.b2Velocity.Length();
		if (speed > unit.maxSpeed) {
			unit.b2Velocity.operator*=(unit.maxSpeed / speed);
		}

		unit.rotate(unit.b2Velocity);

		Ogre::Vector3 newPos = unit.getPosition().operator+=(unit.velocity.operator*(0.0166666));			//Mocked Delta Time
		//Ogre::Vector3 newPos = unit.getPosition().operator+=(unit.velocity.operator*(evt.timeSinceLastFrame));		//Real Time

		unit.mBody->SetLinearVelocity(unit.b2Velocity);
		Ogre::Vector3 moveGraphic = Ogre::Vector3(unit.getB2DPosition().x, 0, unit.getB2DPosition().y);
		unit.commandMove(moveGraphic);

	}
}