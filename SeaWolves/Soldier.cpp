#include "Soldier.h"



Soldier::Soldier(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String BradsBitch, Ogre::String meshName, Ogre::String unitClass, int ID, b2World* world) 
	: Unit(mScnMgr, startPos, BradsBitch, meshName, unitClass, ID, world) 
{
	attackRange = 50;
	mAttackDamage = 10;
	mHitPoints = 100;
}


Soldier::~Soldier()
{
}
