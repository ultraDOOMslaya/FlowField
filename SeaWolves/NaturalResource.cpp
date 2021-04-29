#include "NaturalResource.h"


NaturalResource::NaturalResource(Ogre::SceneManager* gameSceneManager, Ogre::Vector3 startPos, Ogre::String name, Ogre::String meshName, b2World* world, std::vector<GridSquare*>* impassableTerrain)
{
	mScnMgr = gameSceneManager;
	mName = name;
	mWorld = world;
	isSelected = false;

	Ogre::Entity* naturalResourceEntity = mScnMgr->createEntity(meshName);
	naturalResourceEntity->setQueryFlags(Constants::naturalResourceMask);

	mNaturalResourceNode = mScnMgr->getRootSceneNode()->createChildSceneNode(name, startPos);
	mNaturalResourceNode->rotate(Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y));
	mNaturalResourceNode->setScale(50, 50, 50);
	mNaturalResourceNode->attachObject(naturalResourceEntity);

	mBodyDef.type = b2_staticBody;
	mBodyDef.position.Set(startPos.x, startPos.z);
	mBody = world->CreateBody(&mBodyDef);

	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0); //position, relative to body position
	circleShape.m_radius = (18); //radius - around 7 half the radius of the actual graphic

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;
	mBody->CreateFixture(&fixtureDef);

	Ogre::Vector2 gridCoords = GridUtils::cordNumericalFinder(startPos);

	GridSquare* gridSquare = new GridSquare(gridCoords.x, gridCoords.y);
	impassableTerrain->push_back(gridSquare);

}


NaturalResource::~NaturalResource()
{
}


Ogre::Vector3 NaturalResource::getPosition() {
	return mNaturalResourceNode->getPosition();
}
//----------------------------------------------------------------

b2Vec2 NaturalResource::getB2DPosition() {
	return mBody->GetPosition();
}
//----------------------------------------------------------------