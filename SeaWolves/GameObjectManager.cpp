#include "GameObjectManager.h"



GameObjectManager::GameObjectManager()
{
}


GameObjectManager::~GameObjectManager()
{
}


void GameObjectManager::assignUnitToFormationLocation(int width, int height, std::vector<Unit*> units, PathFinding* path) {
	std::multimap<int, Unit*> unitProximities = std::multimap<int, Unit*>();

	int rowSize = 3;
	int groupSize = 1;
	int iterations = (units.size() + (rowSize - 1)) / rowSize;
	int totalUnits = units.size();
	/*while (groupSize < units.size()) {
		rowSize = rowSize + 2;
		groupSize = rowSize * rowSize;
		iterations++;
	}*/

	for (std::vector<Unit*>::iterator unit = units.begin(); unit != units.end(); ++unit) {
		int proximity_x = std::pow(((*unit)->getB2DPosition().x - width), 2);
		int proximity_y = std::pow(((*unit)->getB2DPosition().y - height), 2);
		int proximity = proximity_x + proximity_y;

		unitProximities.insert(std::pair<int, Unit*>(proximity, (*unit)));

		
		/*for (std::multimap<int, Unit*>::iterator unitProximity = unitProximities.begin(); unitProximity != unitProximities.end(); ++unitProximity) {
			unitProximity->second->debugB2Pos1 = unitProximity->second->b2Destination;
		}*/
		
	}

	
	/* Find how many units we need for the given row */
	std::multimap<int, Ogre::Vector2>::iterator position = path->mappedFormation.begin();
	std::multimap<int, Ogre::Vector2>::iterator topographer = path->mappedFormation.begin();
	int rows = 0;
	int columnCount = 1;
	//while (columns <= iterations) {
	while (rows < iterations) {
	//while(columns < 2) {

		int rowColumnKey = topographer->first;
		int numLocations = 0;
		while (topographer->first == rowColumnKey) {
			numLocations++;
			topographer++;
		}

		int numUnits = 0;
		if (totalUnits > numLocations) {
			numUnits = numLocations;
			totalUnits -= numLocations;
		}
		else {
			numUnits = totalUnits;
		}
		

		/* Grab a numUnits amount of unit proximities */
		std::vector<Unit*> rowUnits = std::vector<Unit*>();
		std::multimap<int, Unit*>::iterator unitProximity = unitProximities.begin();
		unitProximity->second->debugInt1 = numUnits;
		for (int i = 0; i < numUnits; i++) {
			rowUnits.push_back(unitProximity->second);
			unitProximities.erase(unitProximity);
			unitProximity++;
		}


		/* loop through the units and map out every possible combination in the row */
		std::vector<Ogre::Vector2> rowLocations = std::vector<Ogre::Vector2>();
		//int count = 0;
		//while (numUnits >= count) {

		//	/* grab all row locations for a single row */
		//	while (position->first == (count + 1)) { //TODO fix this off by 1 error its confusing and lazy
		//		rowLocations.push_back(position->second);
		//		position++;
		//	}

		//	count++;
		//}

		
		while (position->first == columnCount) {
			rowLocations.push_back(position->second);
			position++;
		}
		columnCount++;


		std::multimap<int, std::vector<PotentialUnitLocation*>*> unitLocMaps = std::multimap<int, std::vector<PotentialUnitLocation*>*>();
		for (int x = 0; x < numUnits; x++) {

			int unitCount = 0;
			int locationCount = 0;

			for (int y = 0; y < numUnits; y++) {

				int totalProximity = 0;
				std::vector<PotentialUnitLocation*>* unitsForRow = new std::vector<PotentialUnitLocation*>();
				std::vector<Ogre::Vector2>::iterator location = rowLocations.begin();
				std::vector<Unit*>::iterator unit = rowUnits.begin();

				for (int z = 0; z < numUnits; z++) {
					if (location == rowLocations.end()) {
						break;
					}
					int proximity_x = std::pow(((*unit)->getB2DPosition().x - location->x), 2);
					int proximity_y = std::pow(((*unit)->getB2DPosition().y - location->y), 2);
					int proximity = proximity_x + proximity_y;

					PotentialUnitLocation* unitLocation = new PotentialUnitLocation();
					unitLocation->unit = (*unit);
					unitLocation->potentialLocation = (*location);
					unitLocation->distance = proximity;

					unitsForRow->push_back(unitLocation);
					totalProximity += proximity;

					location++;
					unit++;
				}

				unitLocMaps.insert(std::pair<int, std::vector<PotentialUnitLocation*>*>(totalProximity, unitsForRow));
				std::rotate(rowUnits.begin(), rowUnits.end() - 1, rowUnits.end());
			}
			std::rotate(rowLocations.begin(), rowLocations.end() - 1, rowLocations.end());
		}


		for (std::vector<PotentialUnitLocation*>::iterator shortestLocations = unitLocMaps.begin()->second->begin(); shortestLocations < unitLocMaps.begin()->second->end(); shortestLocations++) {
			b2Vec2 finalPosition = GridUtils::b2NumericalCordFinder((*shortestLocations)->potentialLocation);
			(*shortestLocations)->unit->b2FinalDestination = finalPosition;

			(*shortestLocations)->unit->finalDestination = Ogre::Vector3(finalPosition.x, 0, finalPosition.y);
			(*shortestLocations)->unit->debugPos1 = (*shortestLocations)->potentialLocation;
		}
		rows++;
	}
}