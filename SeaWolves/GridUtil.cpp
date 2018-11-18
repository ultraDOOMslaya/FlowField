#include "GridUtil.h"



GridUtil::GridUtil()
{
}


GridUtil::~GridUtil()
{
}



void GridUtil::getNeighbors(SquareNeighbor squareNeighbor, std::queue<SquareNeighbor*>* squareNeighbors) {
	int num = 0;
	int x, y;
	x = squareNeighbor.getPosition().x;
	y = squareNeighbor.getPosition().y;
	bool topEdge = false;
	bool bottomEdge = false;
	bool leftEdge = false;
	bool rightEdge = false;
	std::queue<Ogre::Vector2> neighbors;
	if ((x - 1) < 0) {
		leftEdge = true;
	}
	else if ((x + 1) > Constants::dimension) {
		rightEdge = true;
	}

	if ((y - 1) < 0) {
		topEdge = true;
	}
	else if ((y + 1) > Constants::dimension) {
		bottomEdge = true;
	}
	if (topEdge && leftEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (topEdge && rightEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
	}
	else if (bottomEdge && leftEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (bottomEdge && rightEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
	}
	else if (topEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (bottomEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (leftEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
		neighbors.push(Ogre::Vector2(x, y + 1));
	}
	else if (rightEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x, y + 1));
	}
	else {
		//all neighbors are valid
		//neighbors = new Ogre::Vector2[9];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}

	while (!neighbors.empty()) {
		Ogre::Vector2 neighbor = neighbors.front();
		int x, y;
		x = neighbor.x;
		y = neighbor.y;
		Ogre::Vector2 neighborPoint = Ogre::Vector2(x, y);

		SquareNeighbor* squareNeighbor = new SquareNeighbor(neighborPoint, 0);

		squareNeighbors->push(squareNeighbor);
		neighbors.pop();
	}
}
//----------------------------------------------------------------

void GridUtil::getAllNeighbors(SquareNeighbor squareNeighbor, std::queue<SquareNeighbor*>* squareNeighbors) {
	int num = 0;
	int x, y;
	x = squareNeighbor.getPosition().x;
	y = squareNeighbor.getPosition().y;
	bool topEdge = false;
	bool bottomEdge = false;
	bool leftEdge = false;
	bool rightEdge = false;
	std::queue<Ogre::Vector2> neighbors;
	if ((x - 1) < 0) {
		leftEdge = true;
	}
	else if ((x + 1) > Constants::dimension) {
		rightEdge = true;
	}

	if ((y - 1) < 0) {
		topEdge = true;
	}
	else if ((y + 1) > Constants::dimension) {
		bottomEdge = true;
	}
	if (topEdge && leftEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (topEdge && rightEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x - 1, y + 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
	}
	else if (bottomEdge && leftEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (bottomEdge && rightEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
	}
	else if (topEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x - 1, y + 1));
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (bottomEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x - 1, y - 1));
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (leftEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
		neighbors.push(Ogre::Vector2(x + 1, y + 1));
		neighbors.push(Ogre::Vector2(x, y + 1));
	}
	else if (rightEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x - 1, y + 1));
		neighbors.push(Ogre::Vector2(x, y + 1));
	}
	else {
		//all neighbors are valid
		//neighbors = new Ogre::Vector2[9];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x - 1, y + 1));
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
		neighbors.push(Ogre::Vector2(x + 1, y - 1));
	}

	while (!neighbors.empty()) {
		Ogre::Vector2 neighbor = neighbors.front();
		int x, y;
		x = neighbor.x;
		y = neighbor.y;
		Ogre::Vector2 neighborPoint = Ogre::Vector2(x, y);

		SquareNeighbor* squareNeighbor = new SquareNeighbor(neighborPoint, 0);

		squareNeighbors->push(squareNeighbor);
		neighbors.pop();
	}
}
//----------------------------------------------------------------

/*
std::queue<SquareNeighbor*> GridUtil::getAllNeighbors(Unit* unit, std::vector<std::vector<int>> dijkastraGrid) {
	int num = 0;
	int x, y;
	x = unit->currentPos.x;
	y = unit->currentPos.y;
	bool topEdge = false;
	bool bottomEdge = false;
	bool leftEdge = false;
	bool rightEdge = false;
	std::queue<Ogre::Vector2> neighbors;
	std::queue<SquareNeighbor*> unitNeighbors;
	if ((x - 1) < 0) {
		leftEdge = true;
	}
	else if ((x + 1) > Constants::dimension) {
		rightEdge = true;
	}

	if ((y - 1) < 0) {
		topEdge = true;
	}
	else if ((y + 1) > Constants::dimension) {
		bottomEdge = true;
	}
	if (topEdge && leftEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (topEdge && rightEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
	}
	else if (bottomEdge && leftEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (bottomEdge && rightEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
	}
	else if (topEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (bottomEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (leftEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
		neighbors.push(Ogre::Vector2(x, y + 1));
	}
	else if (rightEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x, y + 1));
	}
	else {
		//all neighbors are valid
		//neighbors = new Ogre::Vector2[9];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}

	while (!neighbors.empty()) {
		Ogre::Vector2 neighbor = neighbors.front();
		int x, y;
		x = neighbor.x;
		y = neighbor.y;
		Ogre::Vector2 neighborPoint = Ogre::Vector2(x, y);
		int distance = dijkastraGrid[x][y];
		SquareNeighbor* unitNeighbor = new SquareNeighbor(neighborPoint, distance);

		unitNeighbors.push(unitNeighbor);
		neighbors.pop();
	}
	return unitNeighbors;
}*/
//----------------------------------------------------------------