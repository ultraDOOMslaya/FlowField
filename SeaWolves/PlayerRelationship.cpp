#include "PlayerRelationship.h"



PlayerRelationship::PlayerRelationship(int ID, PlayerRelationshipStatus status)
	: friendly(false),
	foe(false)
{
	mID = ID;
	switch (status)
	{
	case PlayerRelationshipStatus::FRIENDLY: friendly = true;
		break;
	case PlayerRelationshipStatus::HOSTILE: foe = true;
		break;
	default:
		break;
	}
}


PlayerRelationship::~PlayerRelationship()
{
}


bool PlayerRelationship::isFriendly() {
	return friendly;
}
//----------------------------------------------------------------

bool PlayerRelationship::isFoe() {
	return foe;
}
//----------------------------------------------------------------

int PlayerRelationship::getID() {
	return mID;
}
//----------------------------------------------------------------

bool PlayerRelationship::hasID(int ID) {
	if (getID() == ID) {
		return true;
	}
	return false;
}
//----------------------------------------------------------------
