#pragma once

#include "Ogre.h"

#include "Constants.h"
#include "SquareNeighbor.h"
#include "GridUtil.h"
#include "Unit.h"

class PathFinding
{
	void PathFinding::calculateLos(SquareNeighbor at, SquareNeighbor pathEnd);

public:
	PathFinding(Ogre::Vector2 squareIndex);
	PathFinding();
	~PathFinding();

	virtual void generateDijkastraGrid(Ogre::Vector2 point);
	virtual void generateFlowField();

	std::vector<std::vector<int>>				dijkastraGrid;
	std::vector<std::vector<Ogre::Vector2>>		flowField;
	bool										flowFieldLock;
	std::vector<std::vector<bool>>				losGrid;
	std::queue<SquareNeighbor*>					squareNeighbors;

	int											pathingUnits;
};

