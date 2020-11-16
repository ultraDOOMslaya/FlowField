#include "WalkingState.h"




void WalkingState::enter(Unit& unit) {
	unit.mState = Unit::STATE_MARCHING;
	unit.animate("Walk");
}


void WalkingState::handleInput(Unit& unit, OgreBites::Event& evt) {
	unit.mState = Unit::STATE_MARCHING;
	unit.mWalkingState->enter(unit);
}

void WalkingState::update(Unit& unit, const Ogre::FrameEvent& evt) {

	/* Realtime directing */
	Ogre::Vector2 position = GridUtils::b2CordNumericalFinder(unit.getB2DPosition());
	if (position != unit.currentPos) {
		unit.currentPos = position;
	}

	/** If the group has LOS, assign the most final destination **/
	// This assigns units into formation if we have LOS
	/*if (unit.mTarget == NULL) {
		if (unit.assignedPathLosDiscovered()) {
			unit.mPreviousState = unit.mState;
			unit.mState = Unit::STATE_FINISH_JOURNEY;
		}
		else if (unit.mState == Unit::STATE_FINISH_JOURNEY) {
			unit.mState = unit.mPreviousState;
		}
	}*/

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

		unit.b2ForceToApply.operator*=(evt.timeSinceLastFrame);		//Real Time
		//unit.b2ForceToApply.operator*=(0.0166666);			//Mocked Delta Time
		unit.b2Velocity.operator+=(unit.b2ForceToApply);

		float speed = unit.b2Velocity.Length();
		if (speed > unit.maxSpeed) {
			unit.b2Velocity.operator*=(unit.maxSpeed / speed);
		}

		unit.rotate(unit.b2Velocity);

		//Ogre::Vector3 newPos = unit.getPosition().operator+=(unit.velocity.operator*(0.0166666));			//Mocked Delta Time
		Ogre::Vector3 newPos = unit.getPosition().operator+=(unit.velocity.operator*(evt.timeSinceLastFrame));		//Real Time

		unit.mBody->SetLinearVelocity(unit.b2Velocity);
		Ogre::Vector3 moveGraphic = Ogre::Vector3(unit.getB2DPosition().x, 0, unit.getB2DPosition().y);
		unit.commandMove(moveGraphic);

	}
}