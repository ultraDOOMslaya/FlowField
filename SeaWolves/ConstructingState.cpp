#include "ConstructingState.h"


void ConstructingState::enter(Unit& unit) {
	unit.mState = Unit::STATE_CONSTRUCTING;
	unit.animate("Building");
}


void ConstructingState::handleInput(Unit& unit, OgreBites::Event& evt) {
	unit.mState = Unit::STATE_MARCHING;
	unit.mWalkingState->enter(unit);
}

void ConstructingState::update(Unit& unit, const Ogre::FrameEvent& evt) {
	unit.build();
	
	unit.mBuildTarget->mTicksToCompletion--;
	if (unit.mBuildTarget->mTicksToCompletion == 0) {

		unit.mBuildTarget->buildingNode->detachAllObjects();
		unit.mBuildTarget->buildingNode->setScale(10, 10, 10);
		unit.mBuildTarget->buildingNode->attachObject(unit.mBuildTarget->buildingEntity);

		unit.mIdleState->enter(unit);
	}

	unit.animationElapsedTime = unit.unitAnimState->getTimePosition();

	//Make sure the graphic is following the physics body
	Ogre::Vector3 moveGraphic = Ogre::Vector3(unit.getB2DPosition().x, unit.getPosition().y, unit.getB2DPosition().y);
	unit.commandMove(moveGraphic);
}
