#include "IdleState.h"


void IdleState::enter(Unit& unit) {
	unit.mState = Unit::STATE_IDLE;
	unit.animate("Idle");
}


void IdleState::handleInput(Unit& unit, OgreBites::Event& evt) {
	unit.mState = Unit::STATE_MARCHING;
	unit.mWalkingState->enter(unit);
}


void IdleState::update(Unit& unit) {
	unit.b2Velocity(0.0f);
	unit.mBody->SetLinearVelocity(b2Vec2_zero);
	unit.mBody->SetType(b2_staticBody);
	unit.trekking = false;
}