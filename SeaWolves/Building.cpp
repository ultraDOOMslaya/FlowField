#include "Building.h"


Building::Building(Ogre::SceneManager* gameSceneManager, Ogre::Vector3 startPos, Ogre::String name, Ogre::String meshName, b2World* world, std::vector<GridSquare*>* impassableTerrain)
{
	mScnMgr = gameSceneManager;
	mName = name;

	Ogre::Entity* buildingEntity = mScnMgr->createEntity(meshName);
	buildingEntity->setQueryFlags(Constants::buildingQueryMask);
	//buildingEntity->setCastShadows(true);

	buildingNode = mScnMgr->getRootSceneNode()->createChildSceneNode(name, startPos);
	buildingNode->rotate(Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y));
	buildingNode->setScale(10, 10, 10);
	buildingNode->attachObject(buildingEntity);

	mBodyDef.type = b2_staticBody;
	mBodyDef.position.Set(startPos.x, startPos.z);
	mBody = world->CreateBody(&mBodyDef);

	b2PolygonShape polygonShape;
	//TODO I don't think the gridwidth and gridheight are what we want here
	//TODO make this a list of geometries and use the mesh vertices to determine their size
	polygonShape.SetAsBox(120, 120);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;
	mBody->CreateFixture(&fixtureDef);

	Ogre::Vector2 gridCords = GridUtils::cordNumericalFinder(startPos);
	addTerrainValue((gridCords.x - 1), (gridCords.y - 1), mWorld, impassableTerrain);
	addTerrainValue((gridCords.x - 1), gridCords.y, mWorld, impassableTerrain);
	addTerrainValue((gridCords.x - 1), (gridCords.y + 1), mWorld, impassableTerrain);

	addTerrainValue(gridCords.x, (gridCords.y - 1), mWorld, impassableTerrain);
	addTerrainValue(gridCords.x, gridCords.y, mWorld, impassableTerrain);
	addTerrainValue(gridCords.x, (gridCords.y + 1), mWorld, impassableTerrain);

	addTerrainValue((gridCords.x + 1), (gridCords.y - 1), mWorld, impassableTerrain);
	addTerrainValue((gridCords.x + 1), gridCords.y, mWorld, impassableTerrain);
	addTerrainValue((gridCords.x + 1), (gridCords.y + 1), mWorld, impassableTerrain);
}


Building::~Building()
{
}


void Building::addTerrainValue(int x, int y, b2World* world, std::vector<GridSquare*>* impassableTerrain) {
	GridSquare* gridSquare = new GridSquare(x, y);
	impassableTerrain->push_back(gridSquare);
}
//----------------------------------------------------------------

void Building::selected() {
	isSelected = true;
	mSelectionCircle = new SelectionCircle(mScnMgr, buildingNode->getPosition(), 100070, 150);
}
//----------------------------------------------------------------

void Building::unselected() {
	isSelected = false;
	delete mSelectionCircle;
}
//----------------------------------------------------------------

void Building::setSpawnPoint(Ogre::Vector2 coords) {
	mSpawnCoords = coords;
}

Ogre::Vector2 Building::getSpawnPoint() {
	return mSpawnCoords;
}