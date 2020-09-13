#include "Archer.h"



Archer::Archer(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String BradsBitch, Ogre::String meshName, Ogre::String unitClass, int ID, b2World* world)
	: Unit(mScnMgr, startPos, BradsBitch, meshName, unitClass, ID, world)
{
	attackRange = 250;
	mAttackDamage = 15;
	mHitPoints = 75;
}


Archer::~Archer()
{
}
