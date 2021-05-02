#pragma once

#include "Ogre.h"

#include "Constants.h"
#include "SquareNeighbor.h"
#include "PathFinding.h"

#include "Box2D\Box2D.h"

#include <exception>
#include <iostream>

class GridUtils
{
public:
	GridUtils();
	~GridUtils();
	// grid value conversion helpers
	static Ogre::Vector2 gridSquareCordFinder(std::string squareName);
	static Ogre::Vector2 numericalCordFinder(Ogre::Vector2 cordinates);
	static b2Vec2 b2NumericalCordFinder(Ogre::Vector2 cordinates);
	static Ogre::Vector3 numericalCordFinder(int x, int y);
	static Ogre::Vector3 numericalCordFinder(Ogre::Vector3 cordinates);
	static Ogre::Vector2 cordNumericalFinder(Ogre::Vector3 position);
	static Ogre::Vector2 b2CordNumericalFinder(b2Vec2 position);
	static Ogre::Vector2 gridIndexFinder(Ogre::String squareName);
	// non grid related helpers
	static int distanceTo(Ogre::Vector3 unit1, Ogre::Vector3 unit2);
	// pathfinding helpers
	static void getNeighbors(SquareNeighbor point, std::queue<SquareNeighbor*>* squareNeighbors);
	static void getAllNeighbors(Ogre::Vector2 point, std::queue<SquareNeighbor*>* squareNeighbors, std::vector<std::vector<int>> dijkastraGrid);
	static void getBasicFormationLocations(int x, int y, int unitCount, std::queue<Ogre::Vector2*>* formationLocations, std::vector<std::vector<int>> dijkastraGrid);
	// unit and terrain collision helpers
	static std::queue<SquareNeighbor*> getTerrainNeighbors(Ogre::Vector2 point, 
														   std::queue<SquareNeighbor*> squareNeighbors, 
														   std::vector<std::vector<int>>* dijkastraGrid);
};

