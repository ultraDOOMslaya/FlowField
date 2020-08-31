#include "Projectile.h"



Projectile::Projectile(Unit* unit, Ogre::SceneManager* mScnMgr, b2World* world)
{
	SceneManager = mScnMgr;
	Ogre::Vector3 projectilePos = Ogre::Vector3(unit->unitNode->getPosition().x, unit->unitNode->getPosition().y + 50, unit->unitNode->getPosition().z);
	Ogre::Vector3 targetPos = Ogre::Vector3(unit->mTarget->unitNode->getPosition().x, unit->mTarget->unitNode->getPosition().y + 50, unit->mTarget->unitNode->getPosition().z);
	projectileEntity = SceneManager->createEntity("Sword.mesh");
	projectileName = unit->unitName + "_sword" + std::to_string(unit->projectileCount);
	unit->projectileCount++;
	unit->projectileEntity = projectileEntity;
	mFletcher = unit;

	projectileNode = SceneManager->getRootSceneNode()->createChildSceneNode(projectileName, projectilePos);
	projectileNode->setScale(5, 5, 5);
	projectileNode->attachObject(projectileEntity);
	mTarget = unit->mTarget;
	mDamage = unit->mAttackDamage;

	Ogre::Vector3 directionToFire = unit->mTarget->getPosition() - unit->getPosition();
	//Rotation code
	Ogre::Vector3 src = projectileNode->getOrientation() * Ogre::Vector3::UNIT_X;

	///* Unit rotation code */
	if ((1.0 + src.dotProduct(directionToFire)) < 0.0001) {
		projectileNode->yaw(Ogre::Degree(180));
	}
	else {
		Ogre::Quaternion quat = src.getRotationTo(directionToFire);
		projectileNode->rotate(quat);
	}

	projectileNode->yaw(Ogre::Degree(90));

	mDestination = targetPos;
	mDirection = mDestination - projectileNode->getPosition();
	mDistance = mDirection.normalise();
}
//----------------------------------------------------------------

Projectile::~Projectile()
{
}
//----------------------------------------------------------------

void Projectile::dealDamage() {
	mTarget->takeDamage(mDamage);

	if (mTarget->mHitPoints <= 0) {
		mFletcher->resetTarget();
	}
}
//----------------------------------------------------------------

void Projectile::halt() {
	Ogre::MovableObject* man = SceneManager->getRootSceneNode()->getAttachedObject(projectileName);
	man->detachFromParent();
	//TODO do this in this classes destructor
	SceneManager->destroyMovableObject(man);
}