#include "GridSquare.h"



GridSquare::GridSquare(b2World* world, int x, int y)
	: x(x),
	y(y)
{
	bodyDef.type - b2_staticBody;
	Ogre::Vector3 cords = GridUtils::numericalCordFinder(x, y);
	bodyDef.position.Set(cords.x, cords.z);
	mBody = world->CreateBody(&bodyDef);

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(Constants::gridHeight, Constants::gridWidth);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;
	mBody->CreateFixture(&fixtureDef);
}


GridSquare::GridSquare(int x, int y, Ogre::String name, Ogre::ColourValue color, Ogre::MaterialPtr defaultMaterial)
	: x(x),
	y(y),
	entityName(name),
	tileColor(color)
{
	mDefaultMaterial = defaultMaterial;
}


GridSquare::~GridSquare()
{
}


void GridSquare::defaultColor(Ogre::SceneManager* gameSceneManager) {

	Ogre::Entity* entity = gameSceneManager->getEntity(entityName);
	entity->setMaterialName(mDefaultMaterial->getName());
}
