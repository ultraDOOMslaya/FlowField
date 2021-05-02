#include "Building.h"


Building::Building(Ogre::SceneManager* gameSceneManager, Ogre::Vector3 startPos, int buildingId, Ogre::String meshName, b2World* world, std::vector<GridSquare*>* impassableTerrain)
	: mTicksToCompletion(300)
{
	mScnMgr = gameSceneManager;
	mBuildingId = buildingId;
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
	mSelectionCircle = new SelectionCircle(mScnMgr, buildingNode->getPosition(), mBuildingId, Constants::GameObject::GAME_OBJECT_BUILDING, 150);
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
//----------------------------------------------------------------

Ogre::Vector2 Building::getSpawnPoint() {
	return mSpawnCoords;
}
//----------------------------------------------------------------

Ogre::Vector3 Building::getPosition() {
	return buildingNode->getPosition();
}
//----------------------------------------------------------------

b2Vec2 Building::getB2DPosition() {
	return mBody->GetPosition();
}
//----------------------------------------------------------------

Ogre::String Building::getName() {
	return mName;
}
//----------------------------------------------------------------