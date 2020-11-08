#include "MagicAttack.h"


MagicAttack::MagicAttack(Unit* unit, Ogre::SceneManager* mScnMgr)
{
	SceneManager = mScnMgr;
	particleName = unit->unitName + "_light_particle" + std::to_string(unit->projectileCount);
	Ogre::ParticleSystem* lightParticle = SceneManager->createParticleSystem(particleName, "Space/Sun");
	unit->projectileCount++;
	mTarget = unit->mTarget;
	mTargetId = unit->unitID;
	mDamage = unit->mAttackDamage;

	Ogre::Vector3 targetPos = unit->mTarget->getPosition();
	targetPos.y += 75;

	particleNode = SceneManager->getRootSceneNode()->createChildSceneNode(particleName, targetPos);
	particleNode->setScale(10, 10, 10);
	particleNode->attachObject(lightParticle);

	summoner = unit;
}
//----------------------------------------------------------------

MagicAttack::~MagicAttack()
{
}
//----------------------------------------------------------------

void MagicAttack::dealDamage() {
	if (mTarget != NULL) {
		mTarget->takeDamage(mDamage);

		if (mTarget->mHitPoints <= 0) {
			summoner->resetTarget();
		}
	}
}
//----------------------------------------------------------------

void MagicAttack::clearTarget() {
	mTarget = NULL;
}
//----------------------------------------------------------------