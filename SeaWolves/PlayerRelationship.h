#pragma once

#include "Ogre.h"
#include "PlayerRelationshipStatus.h"

class PlayerRelationship
{
public:
	PlayerRelationship(int ID, PlayerRelationshipStatus status);
	~PlayerRelationship();

	virtual	bool isFriendly();
	virtual bool isFoe();
	virtual bool hasID(int ID);
	virtual int getID();

protected:
	bool						friendly;
	bool						foe;
	int							mID;
};

