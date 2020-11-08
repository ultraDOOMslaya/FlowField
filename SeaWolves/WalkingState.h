#pragma once
#ifndef __WalkingState_h_
#define __WalkingState_h_

#include "Unit.h"
#include <OgreInput.h>
#include <OgreBitesConfigDialog.h>

class Unit;

class WalkingState
{
public:
	virtual void enter(Unit& unit);
	virtual void handleInput(Unit& unit, OgreBites::Event& evt);
	virtual void update(Unit& unit, const Ogre::FrameEvent& evt);
};

#endif __WalkingState_h_