#include "Barracks.h"



Barracks::Barracks(Ogre::SceneManager* gameSceneManager, Ogre::Vector3 buildingPos, int buildingId, Ogre::String meshName, b2World* world, std::vector<GridSquare*>* impassableTerrain, int ticksToCompletion)
	: Building(gameSceneManager, buildingPos, buildingId, meshName, world, impassableTerrain)
{
	mMeshName = meshName;
	
	mName = "building" + Ogre::StringConverter::toString(buildingId);
	buildingNode = mScnMgr->getRootSceneNode()->createChildSceneNode(mName, buildingPos);
	buildingNode->rotate(Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y));

	constructionEntity = mScnMgr->createEntity("WoodPallet.mesh");
	buildingEntity = mScnMgr->createEntity(meshName);

	//TODO most of this should be generic to the building class
	if (ticksToCompletion > 0) {
		buildingNode->setScale(30, 30, 30);
		buildingNode->attachObject(constructionEntity);
	}
	else {
		buildingNode->setScale(10, 10, 10);
		buildingNode->attachObject(buildingEntity);
	}
	
	constructionEntity->setQueryFlags(Constants::buildingQueryMask);
	buildingEntity->setQueryFlags(Constants::buildingQueryMask);
	//buildingEntity->setCastShadows(true);

	

	mBodyDef.type = b2_staticBody;
	mBodyDef.position.Set(buildingPos.x, buildingPos.z);
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

	Ogre::Vector2 gridCords = GridUtils::cordNumericalFinder(buildingPos);
	 
	mBuildingSpace[0] = Ogre::Vector2((gridCords.x - 1), (gridCords.y - 1));
	mBuildingSpace[0] = Ogre::Vector2((gridCords.x - 1), gridCords.y);
	mBuildingSpace[0] = Ogre::Vector2((gridCords.x - 1), (gridCords.y + 1));

	mBuildingSpace[0] = Ogre::Vector2(gridCords.x, (gridCords.y - 1));
	mBuildingSpace[0] = Ogre::Vector2(gridCords.x, gridCords.y);
	mBuildingSpace[0] = Ogre::Vector2(gridCords.x, (gridCords.y + 1));

	mBuildingSpace[0] = Ogre::Vector2((gridCords.x + 1), (gridCords.y - 1));
	mBuildingSpace[0] = Ogre::Vector2((gridCords.x + 1), gridCords.y);
	mBuildingSpace[0] = Ogre::Vector2((gridCords.x + 1), (gridCords.y + 1));

	for (Ogre::Vector2 gridCord : mBuildingSpace)
	{
		addTerrainValue(gridCord.x, gridCord.y, mWorld, impassableTerrain);
	}

	setSpawnPoint(Ogre::Vector2(gridCords.x, gridCords.y + 2));
}


Barracks::~Barracks()
{
}

