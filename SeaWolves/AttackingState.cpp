#include "AttackingState.h"


void AttackingState::enter(Unit& unit) {
	unit.animate("Attack");
}


void AttackingState::handleInput(Unit& unit, OgreBites::Event& evt) {

}

void AttackingState::update(Unit& unit, const Ogre::FrameEvent& evt) {
	unit.attack();
	if (unit.mUnitClass == "HeavyArmor" && (unit.unitAnimState->getTimePosition() > (unit.unitAnimState->getLength() * 0.5))) {

		if (!unit.hasAttacked)
			unit.mTarget->takeDamage(unit.mAttackDamage);

		unit.hasAttacked = true;
	}
	if (unit.mUnitClass == "Fletcher" && (unit.unitAnimState->getTimePosition() < unit.animationElapsedTime)) {
		if (!unit.hasAttacked) {
			unit.mUnitController->spawnSpellAction(&unit);
		}

		unit.hasAttacked = true;
	}
	if (unit.mUnitClass == "Caster" && (unit.unitAnimState->getTimePosition() > (unit.unitAnimState->getLength() * 0.8))) {
		if (!unit.hasAttacked) {
			unit.mUnitController->spawnSpellAction(&unit);
		}

		unit.hasAttacked = true;
	}
	unit.animationElapsedTime = unit.unitAnimState->getTimePosition();

	//Make sure the graphic is following the physics body
	Ogre::Vector3 moveGraphic = Ogre::Vector3(unit.getB2DPosition().x, 0, unit.getB2DPosition().y);
	unit.commandMove(moveGraphic);
}

