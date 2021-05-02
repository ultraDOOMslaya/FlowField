#include "HuntingState.h"


void HuntingState::enter(Unit& unit) {
	unit.mState = Unit::STATE_HUNTING;
	unit.animate("Walk");
}


void HuntingState::handleInput(Unit& unit, OgreBites::Event& evt) {

}


void HuntingState::update(Unit& unit, const Ogre::FrameEvent& evt) {
	unit.distanceFromTarget = unit.getPosition().squaredLength() - unit.mNatResourceTarget->mNaturalResourceNode->getPosition().squaredLength();
	//TODO switch to resource target
	if (unit.inRange(unit.mNatResourceTarget->getPosition(), unit.harvestRange)) {
		unit.mState = Unit::STATE_IDLE;
		unit.mHarvestingState->enter(unit);
	}
	else {
		unit.finalDestination = unit.mNatResourceTarget->getPosition();
		unit.b2FinalDestination = unit.mNatResourceTarget->getB2DPosition();
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

	/** Marching and Seeking **/
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
		Ogre::Vector3 moveGraphic = Ogre::Vector3(unit.getB2DPosition().x, unit.getPosition().y, unit.getB2DPosition().y);
		unit.commandMove(moveGraphic);

	}
}
