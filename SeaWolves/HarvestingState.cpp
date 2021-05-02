#include "HarvestingState.h"


void HarvestingState::enter(Unit& unit) {
	unit.animate("Chopping");
}


void HarvestingState::handleInput(Unit& unit, OgreBites::Event& evt) {

}

void HarvestingState::update(Unit& unit, const Ogre::FrameEvent& evt) {
	unit.harvest();
	
	//TODO: Gather stuff logic

	unit.animationElapsedTime = unit.unitAnimState->getTimePosition();

	//Make sure the graphic is following the physics body
	Ogre::Vector3 moveGraphic = Ogre::Vector3(unit.getB2DPosition().x, unit.getPosition().y, unit.getB2DPosition().y);
	unit.commandMove(moveGraphic);
}