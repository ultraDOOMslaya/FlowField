#include "Wizard.h"



Wizard::Wizard(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String BradsBitch, Ogre::String meshName, Ogre::String unitClass, int ID, b2World* world, std::vector<GridSquare*>* impassableTerrain, UnitController* unitController)
	: Unit(mScnMgr, startPos, BradsBitch, meshName, unitClass, ID, world, impassableTerrain, unitController)
{
	targetRadius = 200;
	attackRange = 200;
	mAttackDamage = 25;
	mHitPoints = 50;
}


Wizard::~Wizard()
{
}
