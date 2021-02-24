#include "Building.h"



Building::Building(Ogre::SceneManager* gameSceneManager, Ogre::Vector3 startPos, Ogre::String name, Ogre::String meshName, b2World* world, std::vector<GridSquare*>* impassableTerrain)
{
	mScnMgr = gameSceneManager;

	Ogre::Entity* buildingEntity = mScnMgr->createEntity(meshName);
	//buildingEntity->setCastShadows(true);

	Ogre::SceneNode* buildingNode = mScnMgr->getRootSceneNode()->createChildSceneNode(name, startPos);
	buildingNode->rotate(Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y));
	buildingNode->setScale(10, 10, 10);
	buildingNode->attachObject(buildingEntity);
}


Building::~Building()
{
}
