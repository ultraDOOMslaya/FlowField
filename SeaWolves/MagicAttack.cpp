#include "MagicAttack.h"


MagicAttack::MagicAttack(Unit* unit, Ogre::SceneManager* mScnMgr, b2World* world)
{
	SceneManager = mScnMgr;
	particleName = unit->unitName + "_light_particle" + std::to_string(unit->projectileCount);
	Ogre::ParticleSystem* lightParticle = SceneManager->createParticleSystem(particleName, "Space/Sun");
	unit->projectileCount++;
	mTarget = unit->mTarget;
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
	mTarget->takeDamage(mDamage);

	if (mTarget->mHitPoints <= 0) {
		summoner->resetTarget();
	}
}
//----------------------------------------------------------------