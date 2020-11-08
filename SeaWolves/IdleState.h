#pragma once
#ifndef __IdleState_h_
#define __IdleState_h_

#include "Unit.h"
#include <OgreInput.h>
#include <OgreBitesConfigDialog.h>

class Unit;

class IdleState
{
public:
	virtual void enter(Unit& unit);
	virtual void handleInput(Unit& unit, OgreBites::Event& evt);
	virtual void update(Unit& unit);
};

#endif __IdleState_h_