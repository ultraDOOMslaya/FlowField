#pragma once
#ifndef __HarvestingState_h_
#define __HarvestingState_h_

#include "Unit.h"
#include <OgreInput.h>
#include <OgreBitesConfigDialog.h>

class Unit;

class HarvestingState
{
public:
	virtual void enter(Unit& unit);
	virtual void handleInput(Unit& unit, OgreBites::Event& evt);
	virtual void update(Unit& unit, const Ogre::FrameEvent& evt);
};

#endif __HarvestingState_h_