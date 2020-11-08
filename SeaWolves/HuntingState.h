#pragma once
#ifndef __HuntingState_h_
#define __HuntingState_h_

#include "Unit.h"
#include <OgreInput.h>
#include <OgreBitesConfigDialog.h>

class Unit;

class HuntingState
{
public:
	virtual void enter(Unit& unit);
	virtual void handleInput(Unit& unit, OgreBites::Event& evt);
	virtual void update(Unit& unit, const Ogre::FrameEvent& evt);
};

#endif __HuntingState_h_