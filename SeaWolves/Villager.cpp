#include "Villager.h"



Villager::Villager(Ogre::SceneManager* mScnMgr, Ogre::Vector3 startPos, Ogre::String BradsBitch, Ogre::String meshName, Ogre::String unitClass, int ID, b2World* world, std::vector<GridSquare*>* impassableTerrain, UnitController* unitController)
	: Unit(mScnMgr, startPos, BradsBitch, meshName, unitClass, ID, world, impassableTerrain, unitController)
{
	attackRange = 20;
	mAttackDamage = 1;
	mHitPoints = 50;

	mWorkAxeEntity = mScnMgr->createEntity("Axe.mesh");
	Ogre::Quaternion rotation = Ogre::Quaternion(0, sqrt(0.5), 1, sqrt(0.5));
	Ogre::Vector3 offset = Ogre::Vector3(-0.45, 0.35, -0.65);
	mHandHoldPTagPoint = unitEntity->attachObjectToBone("Hand.Hold.L", mWorkAxeEntity, rotation, offset);
	mHandHoldPTagPoint->setInheritOrientation(true);   // - could make this configurable
	mHandHoldPTagPoint->setInheritParentEntityOrientation(true);   // - could make this configurable

	/** Villager units states **/
	mHuntingState = new HuntingState();
}


Villager::~Villager()
{
}


