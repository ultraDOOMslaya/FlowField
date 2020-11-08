#include "HuntingState.h"


void HuntingState::enter(Unit& unit) {
	unit.mState = Unit::STATE_HUNTING;
}


void HuntingState::handleInput(Unit& unit, OgreBites::Event& evt) {

}

void HuntingState::update(Unit& unit, const Ogre::FrameEvent& evt) {

}
