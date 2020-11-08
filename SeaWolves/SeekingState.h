#pragma once
#ifndef __SeekingState_h_
#define __SeekingState_h_

#include "Unit.h"
#include <OgreInput.h>
#include <OgreBitesConfigDialog.h>

class Unit;

class SeekingState
{
public:
	virtual void enter(Unit& unit);
	virtual void handleInput(Unit& unit, OgreBites::Event& evt);
	virtual void update(Unit& unit, const Ogre::FrameEvent& evt);
};

#endif __SeekingState_h_