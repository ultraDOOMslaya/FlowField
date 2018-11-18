#include "PathFinding.h"



PathFinding::PathFinding(Ogre::Vector2 squareIndex)
	: dijkastraGrid(Constants::dimension, std::vector<int>(Constants::dimension, NULL)),
	flowField(Constants::dimension),
	losGrid(Constants::dimension, std::vector<bool>(Constants::dimension, false)),
	flowFieldLock(false),
	pathingUnits(0)
{
	generateDijkastraGrid(squareIndex);
	generateFlowField();
}

PathFinding::PathFinding()
	: dijkastraGrid(Constants::dimension, std::vector<int>(Constants::dimension, NULL)),
	flowField(Constants::dimension),
	losGrid(Constants::dimension, std::vector<bool>(Constants::dimension, false)),
	flowFieldLock(false),
	pathingUnits(0)
{
}


PathFinding::~PathFinding()
{
}


void PathFinding::calculateLos(SquareNeighbor at, SquareNeighbor pathEnd) {
	float xDif = pathEnd.getPosition().x - at.getPosition().x;
	float yDif = pathEnd.getPosition().y - at.getPosition().y;

	float xDifAbs = std::abs(xDif);
	float yDifAbs = std::abs(yDif);

	bool hasLos = false;

	float xDifOne = (xDif > 0) ? 1 : ((xDif < 0) ? -1 : 0);
	float yDifOne = (yDif > 0) ? 1 : ((yDif < 0) ? -1 : 0);;

	//Check in the x direction
	if (xDifAbs >= yDifAbs) {

		if (losGrid[at.getPosition().x + xDifOne][at.getPosition().y]) {
			hasLos = true;
		}
	}
	//Check in the y direction
	if (yDifAbs >= xDifAbs) {

		if (losGrid[at.getPosition().x][at.getPosition().y + yDifOne]) {
			hasLos = true;
		}
	}

	//If we are not a straight line vertically/horizontally to the exit
	if (yDifAbs > 0 && xDifAbs > 0) {
		//If the diagonal doesn't have LOS, we don't
		if (!losGrid[at.getPosition().x + xDifOne][at.getPosition().y + yDifOne]) {
			hasLos = false;
		}
		else if (yDifAbs == xDifAbs) {
			//If we are an exact diagonal and either straight direction is a wall, we don't have LOS
			if (dijkastraGrid[at.getPosition().x + xDifOne][at.getPosition().y] == Constants::WALL || dijkastraGrid[at.getPosition().x][at.getPosition().y + yDifOne] == Constants::WALL) {
				hasLos = false;
			}
		}
	}
	//It's a definite now
	losGrid[at.getPosition().x][at.getPosition().y] = hasLos;
}
//----------------------------------------------------------------

void PathFinding::generateDijkastraGrid(Ogre::Vector2 point) {

	//dijkastraGrid2d(11, std::vector<int>(11)) = new std::vector<std::vector<int>>(CONSTANTS->dimension, std::vector<int> (CONSTANTS->dimension, NULL));

	for (int x = 0; x < Constants::dimension; x++) {
		//dijkastraGrid2d[x] = std::vector<int>(11);

		for (int y = 0; y < Constants::dimension; y++) {
			dijkastraGrid[x][y] = NULL;
			losGrid[x][y] = false;
		}
	}

	//Set impassable terrain

	dijkastraGrid[4][6] = Constants::WALL;
	dijkastraGrid[4][4] = Constants::WALL;
	dijkastraGrid[4][5] = Constants::WALL;
	dijkastraGrid[1][5] = Constants::WALL;
	dijkastraGrid[3][4] = Constants::WALL;
	dijkastraGrid[6][7] = Constants::WALL;
	dijkastraGrid[6][8] = Constants::WALL;
	dijkastraGrid[5][7] = Constants::WALL;
	dijkastraGrid[5][8] = Constants::WALL;

	//Set point of origin to 0
	int x, y;
	x = point.x;
	y = point.y;
	dijkastraGrid[x][y] = 1;
	losGrid[x][y] = true;
	SquareNeighbor* origin = new SquareNeighbor(Ogre::Vector2(x, y), 1);

	std::queue<SquareNeighbor*> toVisit;
	toVisit.push(origin);

	while (!toVisit.empty()) {
		SquareNeighbor* start = toVisit.front();

		//Calculate if we have LOS
		//Only need to see if don't have LOS if we aren't the end
		if (start->getDistance() != origin->getDistance()) {
			calculateLos(*start, *origin);
		}

		GridUtil::getNeighbors(*start, &squareNeighbors);

		while (!squareNeighbors.empty()) {
			SquareNeighbor* neighbor = squareNeighbors.front();
			int x = neighbor->getPosition().x;
			int y = neighbor->getPosition().y;

			if ((x > -1 && y > -1) && (x < 11 && x < 11)) {
				if (dijkastraGrid[x][y] == 0) {
					neighbor->setDistance(start->getDistance() + 1);
					dijkastraGrid[x][y] = neighbor->getDistance();
					toVisit.push(neighbor);
				}
			}
			squareNeighbors.pop();
		}
		toVisit.pop();
		if (start != origin) {
			delete start;
		}	
	}
	delete origin;
}
//----------------------------------------------------------------

void PathFinding::generateFlowField() {

	flowFieldLock = false;
	//Generate an empty grid, set all places as Vector2.zero, which will stand for no good direction
	for (int x = 0; x < Constants::dimension; x++) {
		std::vector<Ogre::Vector2> vec(Constants::dimension);

		for (int y = 0; y < Constants::dimension; y++) {
			Ogre::Vector2 squareCords = Ogre::Vector2::ZERO;
			vec[y] = squareCords;
		}
		flowField[x] = vec;
	}

	//For each grid square
	for (int x = 0; x < Constants::dimension; x++) {
		for (int y = 0; y < Constants::dimension; y++) {

			//obstacles have no flow value
			//TODO
			/*if (flowField[x][y] == std::numeric_limits<int>::max()) {
			continue;
			}*/

			//TODO read on resource acquisition is initilization
			//std::auto_ptr<Ogre::Vector2> pos(new Ogre::Vector2(x, y));
			//std::auto_ptr<SquareNeighbor> pos(new SquareNeighbor(Ogre::Vector2(x, y), dijkastraGrid2d[x][y]));
			SquareNeighbor pos = SquareNeighbor(Ogre::Vector2(x, y), 0);
			GridUtil::getAllNeighbors(pos, &squareNeighbors);

			Ogre::Vector2 min = Ogre::Vector2::ZERO;
			bool minAltered = false;
			int minDistance = 0;

			while (!squareNeighbors.empty()) {
				SquareNeighbor* neighbor = squareNeighbors.front();
				int nx = neighbor->getPosition().x;
				int ny = neighbor->getPosition().y;

				if (nx < 11 && ny < 11) {
					int distance = dijkastraGrid[nx][ny] - dijkastraGrid[pos.getPosition().x][pos.getPosition().y];
					if (distance < minDistance) {
						min = Ogre::Vector2(nx, ny);
						minDistance = distance;
						minAltered = true;
					}
				}

				squareNeighbors.pop();
				delete neighbor;
			}
			if (minAltered) {
				flowField[x][y] = min;
			}
		}
	}
	flowFieldLock = true;
}
//----------------------------------------------------------------