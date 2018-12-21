#pragma once

#include "Ogre.h"

#include "Constants.h"
#include "SquareNeighbor.h"
#include "GridUtils.h"
#include "Unit.h"

class PathFinding
{
	void PathFinding::calculateLos(SquareNeighbor at, SquareNeighbor pathEnd);

public:
	PathFinding(Ogre::Vector2 squareIndex, std::vector<Ogre::Vector2>* impassableTerrain, Ogre::SceneManager* mScnMgr);
	PathFinding();
	~PathFinding();

	virtual void generateDijkastraGrid(Ogre::Vector2 point, std::vector<Ogre::Vector2>* impassableTerrain);
	virtual void generateFlowField();
	virtual void showFlow(Ogre::SceneManager* mScnMgr);
	virtual void removeFlow(Ogre::SceneManager* mScnMgr);
	virtual bool isValid(int x, int y);

	std::vector<std::vector<int>>				dijkastraGrid;
	std::vector<std::vector<Ogre::Vector2>>		flowField;
	bool										flowFieldLock;
	std::vector<std::vector<bool>>				losGrid;
	std::queue<SquareNeighbor*>					squareNeighbors;
	Ogre::SceneManager*							gameSceneManager;


	int											pathingUnits;
	int											pathLines;
	bool										hasPathed;
	Ogre::String								lineName;
	Ogre::String								pathLineName;
};

