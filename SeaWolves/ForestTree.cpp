#include "ForestTree.h"

unsigned int vertexCount(int n) { return n * 4; }
unsigned int indexCount(int n) { return n * 6; }

ForestTree::ForestTree(Ogre::SceneManager* gameSceneManager, Ogre::Vector3 startPos, Ogre::String name, Ogre::String meshName, b2World* world, std::vector<GridSquare*>* impassableTerrain)
	: NaturalResource(gameSceneManager, startPos, name, meshName, world, impassableTerrain)
{
	mHarvestValue = 500;
}


ForestTree::~ForestTree()
{
}
