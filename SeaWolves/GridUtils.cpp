#include "GridUtils.h"



GridUtils::GridUtils()
{
}


GridUtils::~GridUtils()
{
}



Ogre::Vector2 GridUtils::gridSquareCordFinder(std::string squareName)
{
	int squareNumber = stoi(squareName);
	float fedge = Constants::edgeLength * 0.7f;
	int x = 0;
	int z = 0;
	int xcount = 0;
	int ycount = 0;
	int total = 0;
	bool end = false;
	for (xcount; xcount < Constants::dimension; xcount++) {
		total = xcount;
		while (total <= squareNumber) {
			if (total == squareNumber) {
				end = true;
				break;
			}
			else {
				total += Constants::dimension;
				ycount++;
			}
		}
		if (end) {
			break;
		}
		else {
			ycount = 0;
		}
	}
	x = (xcount * fedge) + (fedge * 0.5);
	z = (ycount * fedge) + (fedge * 1.5);
	Ogre::Vector2 cords(x, z);

	std::ostringstream oss;
	oss << "Object is: " << squareNumber << " X: " << xcount << " Y: " << ycount;
	//mCordPanel->setText(oss.str());
	return cords;
}
//----------------------------------------------------------------

Ogre::Vector2 GridUtils::numericalCordFinder(Ogre::Vector2 cordinates) {
	int x = 0;
	int y = 0;
	float fedge = Constants::edgeLength * 0.7f;
	x = (cordinates.x * fedge) + (fedge * 0.5);
	y = (cordinates.y * fedge) + (fedge * 1.5);
	Ogre::Vector2 cords(x, y);
	return cords;
}
//----------------------------------------------------------------

b2Vec2 GridUtils::b2NumericalCordFinder(Ogre::Vector2 cordinates) {
	int x = 0;
	int y = 0;
	float fedge = Constants::edgeLength * 0.7f;
	x = _CMATH_::abs(cordinates.x * fedge) + (fedge * 0.5);
	y = _CMATH_::abs(cordinates.y * fedge) + (fedge * 1.5);
	b2Vec2 cords(x, y);
	return cords;
}
//----------------------------------------------------------------

Ogre::Vector3 GridUtils::numericalCordFinder(int x, int y) {
	float fedge = Constants::edgeLength * 0.7f;
	x = (x * fedge) + (fedge * 0.5);
	y = (y * fedge) + (fedge * 1.5);
	Ogre::Vector3 cords(x, 0, y);
	return cords;
}
//----------------------------------------------------------------

Ogre::Vector3 GridUtils::numericalCordFinder(Ogre::Vector3 cordinates) {
	int x = 0;
	int z = 0;
	float fedge = Constants::edgeLength * 0.7f;
	x = (cordinates.x * fedge) + (fedge * 0.5);
	z = (cordinates.z * fedge) + (fedge * 1.5);
	Ogre::Vector3 cords(x, 0, z);
	return cords;
}
//----------------------------------------------------------------

/* Converts game position into square cordinates
*/
Ogre::Vector2 GridUtils::cordNumericalFinder(Ogre::Vector3 position) {
	int x = 0;
	int z = 0;
	float fedge = Constants::edgeLength * 0.7f;
	x = (position.x - (fedge * 0.5) + 1) / fedge;		//+1 is a round up to help the int division math
	z = (position.z - (fedge * 1.5) + 1) / fedge;		//otherwise end up with cases where unit movement falls short
	Ogre::Vector2 cords(x, z);
	return cords;
}
//----------------------------------------------------------------

/* Converts box2D position into square cordinates
*/
Ogre::Vector2 GridUtils::b2CordNumericalFinder(b2Vec2 position) {
	float x = 0;
	float y = 0;
	float fedge = Constants::edgeLength * 0.7f;
	x = (position.x - (fedge * 0.5) + 1) / fedge;		//+1 is a round up to help the int division math
	y = (position.y - (fedge * 1.5) + 1) / fedge;		//otherwise end up with cases where unit movement falls short
	Ogre::Vector2 cords(_CMATH_::round(x), _CMATH_::round(y));
	return cords;
}
//----------------------------------------------------------------

Ogre::Vector2 GridUtils::gridIndexFinder(Ogre::String squareName) {
	int squareNumber = stoi(squareName);

	int xcount = 0;
	int ycount = 0;
	int total = 0;
	bool end = false;
	for (xcount; xcount < Constants::dimension; xcount++) {
		total = xcount;
		while (total <= squareNumber) {
			if (total == squareNumber) {
				end = true;
				break;
			}
			else {
				total += Constants::dimension;
				ycount++;
			}
		}
		if (end) {
			break;
		}
		else {
			ycount = 0;
		}
	}
	Ogre::Vector2 squareIndex = Ogre::Vector2(xcount, ycount);
	return squareIndex;
}
//----------------------------------------------------------------

int GridUtils::distanceTo(Ogre::Vector3 object1, Ogre::Vector3 object2) {
	int a = object1.x - object2.x;
	int b = object1.z - object2.z;

	int c = std::sqrt(a*a + b*b);
	return c;
}
//----------------------------------------------------------------


bool isValid(int x, int y, std::vector<std::vector<int>> dijkastraGrid) {
	if ((x <= Constants::gridWidth && x > 0) && (y <= Constants::gridHeight && y > 0)) {
		return x >= 0 && y >= 0 && x < Constants::gridWidth && y < Constants::gridHeight && dijkastraGrid[x][y] != Constants::WALL;
	}
	return false;
}
//----------------------------------------------------------------


void GridUtils::getNeighbors(SquareNeighbor squareNeighbor, std::queue<SquareNeighbor*>* squareNeighbors) {
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

void dissallowIllegalDiagonals(int x, int y, std::queue<Ogre::Vector2>* neighbors, std::vector<std::vector<int>> dijkastraGrid) {
	bool up = isValid(x, y - 1, dijkastraGrid);
	bool down = isValid(x, y + 1, dijkastraGrid);
	bool right = isValid(x - 1, y, dijkastraGrid);
	bool left = isValid(x + 1, y, dijkastraGrid);

}

void GridUtils::getAllNeighbors(Ogre::Vector2 point, std::queue<SquareNeighbor*>* squareNeighbors, std::vector<std::vector<int>> dijkastraGrid) {
	int num = 0;
	int x, y;
	x = point.x;
	y = point.y;
	bool topEdge = false;
	bool bottomEdge = false;
	bool leftEdge = false;
	bool rightEdge = false;
	std::queue<Ogre::Vector2> neighbors;

	/*
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
	*/

	bool up = isValid(x, y - 1, dijkastraGrid);
	bool down = isValid(x, y + 1, dijkastraGrid);
	bool left = isValid(x - 1, y, dijkastraGrid);
	bool right = isValid(x + 1, y, dijkastraGrid);

	if (left) {
		neighbors.push(Ogre::Vector2(x - 1, y));

		if (up && isValid(x - 1, y - 1, dijkastraGrid)) {
			neighbors.push(Ogre::Vector2(x - 1, y - 1));
		}
	}

	if (up) {
		neighbors.push(Ogre::Vector2(x, y - 1));

		if (right && isValid(x + 1, y - 1, dijkastraGrid)) {
			neighbors.push(Ogre::Vector2(x + 1, y - 1));
		}
	}

	if (right) {
		neighbors.push(Ogre::Vector2(x + 1, y));

		if (down && isValid(x + 1, y + 1, dijkastraGrid)) {
			neighbors.push(Ogre::Vector2(x + 1, y + 1));
		}
	}

	if (down) {
		neighbors.push(Ogre::Vector2(x, y + 1));

		if (left && isValid(x - 1, y + 1, dijkastraGrid)) {
			neighbors.push(Ogre::Vector2(x - 1, y + 1));
		}
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

void GridUtils::getBasicFormationLocations(int width, int height, int unitCount, std::queue<Ogre::Vector2*>* formationLocations, std::vector<std::vector<int>> dijkastraGrid) {
	int groupSize = 1;
	int iterations = 0;
	while (groupSize < unitCount) {
		groupSize = groupSize + 2;
		groupSize = groupSize * groupSize;
		iterations++;
	}

	for (int x = (iterations * -1); x <= iterations; x++) {
		for (int y = (iterations * -1); y <= iterations; y++) {
			Ogre::Vector2* formationLocation = new Ogre::Vector2((width+x), (height+y));
			formationLocations->push(formationLocation);
		}
	}
}
//----------------------------------------------------------------

/*
 This one ends up returning because I couldn't figure out how to pass a queue by reference when attached to the unit or pathfinding class.
 On second thought, it may be ideal to return on all of these neighbor methods since the structure they pass back isn't very large.
*/
std::queue<SquareNeighbor*> GridUtils::getTerrainNeighbors(Ogre::Vector2 point, std::queue<SquareNeighbor*> squareNeighbors, std::vector<std::vector<int>>* dijkastraGrid) {
	int num = 0;
	int x, y;
	x = point.x;
	y = point.y;
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
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (topEdge && rightEdge) {
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x - 1, y + 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
	}
	else if (bottomEdge && leftEdge) {
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (bottomEdge && rightEdge) {
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
	}
	else if (topEdge) {
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x - 1, y + 1));
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (bottomEdge) {
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x - 1, y - 1));
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (leftEdge) {
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
		neighbors.push(Ogre::Vector2(x + 1, y + 1));
		neighbors.push(Ogre::Vector2(x, y + 1));
	}
	else if (rightEdge) {
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x - 1, y + 1));
		neighbors.push(Ogre::Vector2(x, y + 1));
	}
	else {
		//all neighbors are valid
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
		int distance = (*dijkastraGrid)[x][y];
		SquareNeighbor* unitNeighbor = new SquareNeighbor(neighborPoint, distance);

		squareNeighbors.push(unitNeighbor);
		neighbors.pop();
	}
	return squareNeighbors;
}
//----------------------------------------------------------------