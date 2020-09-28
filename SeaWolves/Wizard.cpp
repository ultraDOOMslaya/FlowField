#include "Wizard.h"



Wizard::Wizard(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String BradsBitch, Ogre::String meshName, Ogre::String unitClass, int ID, b2World* world, std::vector<GridSquare*>* impassableTerrain)
	: Unit(mScnMgr, startPos, BradsBitch, meshName, unitClass, ID, world, impassableTerrain)
{
	targetRadius = 200;
	attackRange = 200;
	mAttackDamage = 25;
	mHitPoints = 50;
}


Wizard::~Wizard()
{
}
