#pragma once
#ifndef __AttackingState_h_
#define __AttackingState_h_

#include "Unit.h"
#include <OgreInput.h>
#include <OgreBitesConfigDialog.h>

class Unit;

class AttackingState
{
public:
	virtual void enter(Unit& unit);
	virtual void handleInput(Unit& unit, OgreBites::Event& evt);
	virtual void update(Unit& unit, const Ogre::FrameEvent& evt);
};

#endif __AttackingState_h_