#pragma once

#include "Ogre.h"

#include "Constants.h"
#include "Unit.h"
#include "SquareNeighbor.h"

class GridUtil
{
public:
	GridUtil();
	~GridUtil();

	static void getNeighbors(SquareNeighbor point, std::queue<SquareNeighbor*>* squareNeighbors);
	static void getAllNeighbors(SquareNeighbor point, std::queue<SquareNeighbor*>* squareNeighbors);
	//static std::queue<SquareNeighbor*> getAllNeighbors(Unit* unit, std::vector<std::vector<int>> dijkastraGrid);
};

