#include "PathFinding.h"


PathFinding::PathFinding(Ogre::Vector2 squareIndex, std::vector<GridSquare*>* impassableTerrain, Ogre::SceneManager* mScnMgr, int queueSize, Ogre::Vector3 conglomerate)
	: dijkastraGrid(Constants::dimension, std::vector<int>(Constants::dimension, NULL)),
	flowField(Constants::dimension),
	losGrid(Constants::dimension, std::vector<bool>(Constants::dimension, false)),
	flowFieldLock(false),
	pathingUnits(0),
	pathLines(0),
	hasPathed(false),
	losDiscovered(false),
	pathLineName("pathLine0")
{
	origin = squareIndex;
	generateDijkastraGrid(squareIndex, impassableTerrain);
	generateFlowField();
	gameSceneManager = mScnMgr;

	assignUnitFormationLocations(squareIndex.x, squareIndex.y, queueSize, conglomerate);

	lineName = "pathLine";
	pathLineName = "pathLine0";
}


PathFinding::PathFinding()
	: dijkastraGrid(Constants::dimension, std::vector<int>(Constants::dimension, NULL)),
	flowField(Constants::dimension),
	losGrid(Constants::dimension, std::vector<bool>(Constants::dimension, false)),
	flowFieldLock(false),
	pathingUnits(0),
	pathLines(0),
	hasPathed(false),
	losDiscovered(false)
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

	bool lineOfSight = false;

	float xDifOne = (xDif > 0) ? 1 : ((xDif < 0) ? -1 : 0);
	float yDifOne = (yDif > 0) ? 1 : ((yDif < 0) ? -1 : 0);;

	//Check in the x direction
	if (xDifAbs >= yDifAbs) {

		if (losGrid[at.getPosition().x + xDifOne][at.getPosition().y]) {
			lineOfSight = true;
		}
	}
	//Check in the y direction
	if (yDifAbs >= xDifAbs) {

		if (losGrid[at.getPosition().x][at.getPosition().y + yDifOne]) {
			lineOfSight = true;
		}
	}

	//If we are not a straight line vertically/horizontally to the exit
	if (yDifAbs > 0 && xDifAbs > 0) {
		//If the diagonal doesn't have LOS, we don't
		if (!losGrid[at.getPosition().x + xDifOne][at.getPosition().y + yDifOne]) {
			lineOfSight = false;
		}
		else if (yDifAbs == xDifAbs) {
			//If we are an exact diagonal and either straight direction is a wall, we don't have LOS
			if (dijkastraGrid[at.getPosition().x + xDifOne][at.getPosition().y] == Constants::WALL || dijkastraGrid[at.getPosition().x][at.getPosition().y + yDifOne] == Constants::WALL) {
				lineOfSight = false;
			}
		}
	}
	//It's a definite now
	losGrid[at.getPosition().x][at.getPosition().y] = lineOfSight;
}
//----------------------------------------------------------------

bool PathFinding::isValid(int x, int y) {
	return x >= 0 && y >= 0 && x < Constants::gridWidth && y < Constants::gridHeight && dijkastraGrid[x][y] != Constants::WALL;
}
//----------------------------------------------------------------

void PathFinding::generateDijkastraGrid(Ogre::Vector2 point, std::vector<GridSquare*>* impassableTerrain) {

	//dijkastraGrid2d(11, std::vector<int>(11)) = new std::vector<std::vector<int>>(CONSTANTS->dimension, std::vector<int> (CONSTANTS->dimension, NULL));

	for (int x = 0; x < Constants::dimension; x++) {
		//dijkastraGrid2d[x] = std::vector<int>(11);

		for (int y = 0; y < Constants::dimension; y++) {
			dijkastraGrid[x][y] = NULL;
			losGrid[x][y] = false;
		}
	}

	//Set impassable terrain
	for (std::vector<GridSquare*>::iterator terrain = impassableTerrain->begin(); terrain != impassableTerrain->end(); ++terrain) {
		dijkastraGrid[(*terrain)->x][(*terrain)->y] = Constants::WALL;
	}

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

		GridUtils::getNeighbors(*start, &squareNeighbors);

		while (!squareNeighbors.empty()) {
			SquareNeighbor* neighbor = squareNeighbors.front();
			int x = neighbor->getPosition().x;
			int y = neighbor->getPosition().y;

			if ((x > -1 && y > -1) && (x < Constants::dimension && x < Constants::dimension)) {
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
			if (dijkastraGrid[x][y] == Constants::WALL) {
				continue;
			}

			//TODO read on resource acquisition is initilization
			//std::auto_ptr<Ogre::Vector2> pos(new Ogre::Vector2(x, y));
			//std::auto_ptr<SquareNeighbor> pos(new SquareNeighbor(Ogre::Vector2(x, y), dijkastraGrid2d[x][y]));
			SquareNeighbor pos = SquareNeighbor(Ogre::Vector2(x, y), 0);
			GridUtils::getAllNeighbors(pos.getPosition(), &squareNeighbors, dijkastraGrid);

			Ogre::Vector2 min = Ogre::Vector2::ZERO;
			bool minAltered = false;
			int minDistance = 0;

			while (!squareNeighbors.empty()) {
				SquareNeighbor* neighbor = squareNeighbors.front();
				int nx = neighbor->getPosition().x;
				int ny = neighbor->getPosition().y;

				if (nx < Constants::dimension && ny < Constants::dimension) {
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

void PathFinding::alterDijkastraGrid(int x, int y, int value) {
	dijkastraGrid[x][y] = dijkastraGrid[x][y] + value;
	generateFlowField();
}
//----------------------------------------------------------------

void PathFinding::removeFlow(Ogre::SceneManager* mScnMgr) {
	
	//gameSceneManager->getRootSceneNode()->removeAndDestroyChild("pathLine0");
	//gameSceneManager->getRootSceneNode()->detachObject("pathNode0");
	/*
	Ogre::Entity* line = gameSceneManager->getEntity(lineName);
	Ogre::SceneNode* parent = line->getParentSceneNode();
	parent->detachObject(line);
	gameSceneManager->destroyEntity(line);
	gameSceneManager->destroySceneNode(parent);
	*/
	int area = Constants::dimension * Constants::dimension;
	for (int i = 0; i < area; i++) {
		Ogre::ManualObject* lineMO = gameSceneManager->getManualObject(lineName + Ogre::StringConverter::toString(i));
		Ogre::SceneNode* parent = lineMO->getParentSceneNode();
		parent->detachObject(lineMO);
		gameSceneManager->destroyManualObject(lineMO);
	}
}
//----------------------------------------------------------------

void PathFinding::showFlow(Ogre::SceneManager* mScnMgr) {
	//int previousPathLine = 0;
	//Ogre::String destroyThisLine = lineName + Ogre::StringConverter::toString(previousPathLine);
	if (gameSceneManager->hasManualObject(pathLineName)) {
		
		removeFlow(mScnMgr);
		/*
		Ogre::ManualObject* lineMO = gameSceneManager->getManualObject(lineName + Ogre::StringConverter::toString(previousPathLine));
		Ogre::SceneNode* parent = lineMO->getParentSceneNode();
		parent->detachObject(lineMO);
		gameSceneManager->destroyManualObject(lineMO);
		*/
	}
	
	/*
	Ogre::Vector3 origin = GridUtils::numericalCordFinder(1, 1);
	Ogre::Vector3 endpoint = GridUtils::numericalCordFinder(flowField[1][1].x, flowField[1][1].y);
	Ogre::String pathLineName = lineName + Ogre::StringConverter::toString(previousPathLine);

	Ogre::ManualObject* man = gameSceneManager->createManualObject(pathLineName);
	man->begin("Examples/OgreLogo", Ogre::RenderOperation::OT_LINE_STRIP);

	man->position(origin.x, 1, origin.z);
	man->normal(0, 0, 1);

	man->position(endpoint.x, 1, endpoint.z);
	man->normal(0, 0, 1);

	man->end();
	gameSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(man);
	*/

	for (int x = 0; x < Constants::dimension; x++) {
		for (int y = 0; y < Constants::dimension; y++) {
			Ogre::Vector3 origin = GridUtils::numericalCordFinder(x, y);
			Ogre::Vector3 endpoint;
			if (flowField[x][y] == Ogre::Vector2::ZERO) {
				endpoint = origin;
			}
			else {
				endpoint = GridUtils::numericalCordFinder(flowField[x][y].x, flowField[x][y].y);
			}
				
			Ogre::String pathLineName = lineName + Ogre::StringConverter::toString(pathLines);
			Ogre::ManualObject* man = mScnMgr->createManualObject(pathLineName);
			man->begin("Examples/OgreLogo", Ogre::RenderOperation::OT_LINE_STRIP);

			man->position(origin.x, 205, origin.z);
			man->normal(0, 0, 1);

			int interpolateX = (std::abs(origin.x) + std::abs(endpoint.x)) / 2;
			int interpolateY = (std::abs(origin.z) + std::abs(endpoint.z)) / 2;
			man->position(interpolateX, 205, interpolateY);
			man->normal(0, 0, 1);

			man->end();
			mScnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(man);
			pathLines++;		
		}
	}
	
}
//----------------------------------------------------------------

/** Find location formation coords for a given number of units based on a width and height coord. **/
//TODO: Now that we have code to actually assign a unit its final coords, rework this so it can handle more than 9 locations
void PathFinding::assignUnitFormationLocations(int width, int height, int numUnits, Ogre::Vector3 conglomerate) {
	//GridUtils::getBasicFormationLocations(x, y, numUnits, &formationLocations, dijkastraGrid);
	int rowSize = 1;
	int groupSize = 1;
	int iterations = 0;
	while (groupSize < numUnits) {
		rowSize = rowSize + 2;
		groupSize = rowSize * rowSize;
		iterations++;
	}


	
	Ogre::Radian angle = conglomerate.angleBetween(GridUtils::numericalCordFinder(Ogre::Vector3(width, 0, height)));
	Ogre::Real degree = angle.valueDegrees();
	std::map<int, Ogre::Vector2*> distanceLocations;

	/*for (int x = (iterations * -1); x <= iterations; x++) {
		for (int y = (iterations * -1); y <= iterations; y++) {
			Ogre::Vector2* formationLocation = new Ogre::Vector2((width + x), (height + y));
			//formationLocations.push_back(formationLocation);
			int distance = GridUtils::distanceTo(GridUtils::numericalCordFinder(formationLocation->x, formationLocation->y), conglomerate);
			distanceLocations.insert(std::pair<int, Ogre::Vector2*>(distance, formationLocation));
			mappedFormation.insert(std::pair<int, int>(x, y));
		}
	}*/
	/*
	//TODO: Or just use a stack instead of a queue
	for (std::map<int, Ogre::Vector2*>::reverse_iterator riter = distanceLocations.rbegin(); riter != distanceLocations.rend(); riter++) {
	//for (std::map<int, Ogre::Vector2*>::iterator iter = distanceLocations.begin(); iter != distanceLocations.end(); iter++) {
		formationLocations.push_back(riter->second);
	}*/

	bool boxFormation = false;
	if ((degree < 31.0f) || (degree > 60.0f)) {
		boxFormation = true;
	}
	if ((degree < 31.0f) || (degree > 60.0f)) {
		Ogre::Vector3 pointToMoveTo = GridUtils::numericalCordFinder(Ogre::Vector3(width, 0, height));
		int xOffset = 1;
		int yOffset = 1;
		int offset = 1;
		bool leftRight = false;
		bool topDown = false;
		if (pointToMoveTo.x < conglomerate.x) {
			xOffset = -1;
		}
		if (pointToMoveTo.z < conglomerate.z) {
			yOffset = -1;
		}

		if (std::abs(pointToMoveTo.x - conglomerate.x) < std::abs(pointToMoveTo.z - conglomerate.z)) {
			xOffset = 0;
			topDown = true;
		}
		else {
			yOffset = 0;
			leftRight = true;
		}

		int startingOffset = rowSize * 0.5; //Works as intended
		int pointToMoveToOffsetX = width + (startingOffset * xOffset);
		int pointToMoveToOffsetY = height + (startingOffset * yOffset);

		for (int a = 1; a <= rowSize; a++) {
			for (int b = 0; b <= iterations; b++) {
				if (b == 0) {
					formationLocations.push_back(new Ogre::Vector2(pointToMoveToOffsetX, pointToMoveToOffsetY));
					mappedFormation.insert(std::pair<int, Ogre::Vector2>(a, Ogre::Vector2(pointToMoveToOffsetX, pointToMoveToOffsetY)));
				}
				else if (topDown){
					formationLocations.push_back(new Ogre::Vector2((pointToMoveToOffsetX + offset), pointToMoveToOffsetY));
					formationLocations.push_back(new Ogre::Vector2((pointToMoveToOffsetX - offset), pointToMoveToOffsetY));

					mappedFormation.insert(std::pair<int, Ogre::Vector2>(a, Ogre::Vector2((pointToMoveToOffsetX + offset), pointToMoveToOffsetY)));
					mappedFormation.insert(std::pair<int, Ogre::Vector2>(a, Ogre::Vector2((pointToMoveToOffsetX - offset), pointToMoveToOffsetY)));
				}
				else if (leftRight) {
					formationLocations.push_back(new Ogre::Vector2(pointToMoveToOffsetX, (pointToMoveToOffsetY - offset)));
					formationLocations.push_back(new Ogre::Vector2(pointToMoveToOffsetX, (pointToMoveToOffsetY + offset)));

					mappedFormation.insert(std::pair<int, Ogre::Vector2>(a, Ogre::Vector2(pointToMoveToOffsetX, (pointToMoveToOffsetY - offset))));
					mappedFormation.insert(std::pair<int, Ogre::Vector2>(a, Ogre::Vector2(pointToMoveToOffsetX, (pointToMoveToOffsetY + offset))));
				}
			}
			pointToMoveToOffsetX -= xOffset;
			pointToMoveToOffsetY -= yOffset;
		}
	}
	else {
		Ogre::Vector3 pointToMoveTo = GridUtils::numericalCordFinder(Ogre::Vector3(width, 0, height));
		int xOffset = -1;
		int yOffset = -1;
		if ((pointToMoveTo.x - conglomerate.x) > 0) {
			xOffset = 1;
		}
		if ((pointToMoveTo.z - conglomerate.z) > 0) {
			yOffset = 1;
		}

		int startingOffset = rowSize * 0.5; //Works as intended
		int pointToMoveToOffsetX = width + (startingOffset * xOffset);
		int pointToMoveToOffsetY = height + (startingOffset * yOffset);

		for (int a = 1; a <= rowSize; a++) {
			for (int b = 0; b <= iterations; b++) {
				if (b == 0) {
					formationLocations.push_back(new Ogre::Vector2(pointToMoveToOffsetX, pointToMoveToOffsetY));
					mappedFormation.insert(std::pair<int, Ogre::Vector2>(a, Ogre::Vector2(pointToMoveToOffsetX, pointToMoveToOffsetY)));
				}
				else {
					formationLocations.push_back(new Ogre::Vector2((pointToMoveToOffsetX + (iterations * xOffset)), (pointToMoveToOffsetY - (iterations * yOffset))));
					formationLocations.push_back(new Ogre::Vector2((pointToMoveToOffsetX - (iterations * xOffset)), (pointToMoveToOffsetY + (iterations * yOffset))));

					mappedFormation.insert(std::pair<int, Ogre::Vector2>(a, Ogre::Vector2((pointToMoveToOffsetX + (iterations * xOffset)), (pointToMoveToOffsetY - (iterations * yOffset)))));
					mappedFormation.insert(std::pair<int, Ogre::Vector2>(a, Ogre::Vector2((pointToMoveToOffsetX - (iterations * xOffset)), (pointToMoveToOffsetY + (iterations * yOffset)))));
				}
			}
			pointToMoveToOffsetX -= xOffset;
			pointToMoveToOffsetY -= yOffset;
		}
	}
}
//----------------------------------------------------------------
