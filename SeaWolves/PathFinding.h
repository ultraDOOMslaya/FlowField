#pragma once

#include "Ogre.h"

#include "GridSquare.h"
#include "Constants.h"
#include "GridUtils.h"
#include "SquareNeighbor.h"

#include "Unit.h"

#include <map>

#define PI 3.14159265

class GridSquare; //Forward declaration

class PathFinding
{
	void PathFinding::calculateLos(SquareNeighbor at, SquareNeighbor pathEnd);

public:
	PathFinding(Ogre::Vector2 squareIndex, std::vector<GridSquare*>* impassableTerrain, Ogre::SceneManager* mScnMgr);
	PathFinding();
	~PathFinding();

	virtual void generateDijkastraGrid(Ogre::Vector2 point, std::vector<GridSquare*>* impassableTerrain);
	virtual void generateFlowField();
	void alterDijkastraGrid(int x, int y, int value);
	virtual void showFlow(Ogre::SceneManager* mScnMgr);
	virtual void removeFlow(Ogre::SceneManager* mScnMgr);
	virtual void assignUnitFormationLocations(int x, int y, int numUnits, Ogre::Vector3 conglomerate);
	virtual bool isValid(int x, int y);

	Ogre::Vector2									   origin;
	std::vector<std::vector<int>>				dijkastraGrid;
	std::vector<std::vector<Ogre::Vector2>>		flowField;
	bool										flowFieldLock;
	std::vector<std::vector<bool>>				losGrid;
	std::queue<SquareNeighbor*>					squareNeighbors;
	std::vector<Ogre::Vector2*>					formationLocations;			//Locations units will end on.
	int											formationDistance;			//Distance until units starting moving into formation.
	Ogre::SceneManager*							gameSceneManager;
	
	int											pathingUnits;
	int											pathLines;
	bool										hasPathed;
	bool										losDiscovered;
	Ogre::String								lineName;
	Ogre::String								pathLineName;

	int											clearPointDistance;
	std::multimap<int, Ogre::Vector2>			mappedFormation;

};

