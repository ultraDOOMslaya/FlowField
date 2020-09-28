#include "UnitController.h"



UnitController::UnitController()
{
}


UnitController::~UnitController()
{
}




void PrintStack(std::stack<Unit*> s, std::vector<Unit*>* partition)
{
	// If stack is empty then return 
	if (s.empty())
		return;


	Unit* unit = s.top();

	// Pop the top element of the stack 
	s.pop();

	// Recursively call the function PrintStack 
	PrintStack(s, partition);

	// Print the stack element starting 
	// from the bottom
	partition->push_back(unit);
	//std::cout << "{" << x.getX() << "," << x.getY() << "}" << " ";

	// Push the same element onto the stack 
	// to preserve the order 
	s.push(unit);
}

static void permutations(std::vector<Unit*>* items, std::stack<Unit*>* permutation, int size, std::vector<std::vector<Unit*>*>* unitCombinations) {

	if (permutation->size() == size) {
		//std::cout << permutation->top();
		std::vector<Unit*>* partition = new std::vector<Unit*>();
		unitCombinations->push_back(partition);
		PrintStack(*permutation, partition);
	}

	std::vector<Unit*> availableItems;
	for (int x = 0; x < items->size(); x++)
		availableItems.push_back(items->at(x));

	for (std::vector<Unit*>::iterator iter = availableItems.begin(); iter < availableItems.end(); ++iter) {
		permutation->push(*iter);

		// remove item from available item set
		//items->erase(std::remove(items->begin(), items->end(), (items->begin() + i)), items->end());
		items->erase(items->begin());

		// pass on for next permutation
		permutations(items, permutation, size, unitCombinations);

		// pop and put the removed item back
		items->push_back(permutation->top());
		permutation->pop();

	}
}

/** by row impl. Efficient but not as good as the shotgun approach **/
void UnitController::priorityQueueFormation(int width, int height, std::vector<Unit*> units, PathFinding* path) {
	int rowSize = 4;
	int iterations = (units.size() + (rowSize - 1)) / rowSize;
	int totalUnits = units.size();
	int allUnitsAssigned = 0;

	int totalProximity = 0;
	std::multimap<int, std::vector<PotentialUnitLocationReference*>*> unitLocMaps = std::multimap<int, std::vector<PotentialUnitLocationReference*>*>();
	std::vector<Ogre::Vector2> rowLocations = std::vector<Ogre::Vector2>();

	std::multimap<int, Ogre::Vector2>::iterator formationSpot = path->mappedFormation.begin();
	for (int i = 0; i < totalUnits; ++i) {
		rowLocations.push_back(Ogre::Vector2(formationSpot->second.x, formationSpot->second.y));
		formationSpot++;
	}
	int totalLocations = rowLocations.size();

	int currentSpot = 0;

	/** wrap units with SharedReference flags **/
	std::vector<SharedUnitReference*> sharedUnits;
	for (auto unit = units.begin(); unit != units.end(); ++unit) {
		SharedUnitReference* sharedUnit = new SharedUnitReference();
		sharedUnit->unit = (*unit);
		sharedUnits.push_back(sharedUnit);
	}

	/** prelimainrily: track where we are in the structure **/
	std::vector<std::vector<std::multimap<int, PotentialUnitLocationReference*>>> rowPermutationPriorityQueue;
	//insertNewRowPriorityMap(&rowPermutationPriorityQueue);

	std::vector<std::multimap<int, PotentialUnitLocationReference*>> rowPriorityMap;
	rowPermutationPriorityQueue.push_back(rowPriorityMap);
	int currentRow = 1;
	int containerPosition = 0;

	/** evaluate each spot and insert the portential units to a given locations priority queue **/
	for (auto rowFormationSpot = path->mappedFormation.begin(); rowFormationSpot != path->mappedFormation.end(); ++rowFormationSpot) {
		
		// Make sure we haven't moved up a row
		if (rowFormationSpot->first > currentRow) {
			currentRow = rowFormationSpot->first;
			//insertNewRowPriorityMap(&rowPermutationPriorityQueue);

			std::vector<std::multimap<int, PotentialUnitLocationReference*>> rowPriorityMap;
			rowPermutationPriorityQueue.push_back(rowPriorityMap);

			containerPosition++;
		}

		SharedLocationReference* sharedLocation = new SharedLocationReference();
		sharedLocation->potentialLocation = rowFormationSpot->second;

		std::multimap<int, PotentialUnitLocationReference*> locationUnitMap;
		for (auto sharedUnit = sharedUnits.begin(); sharedUnit < sharedUnits.end(); ++sharedUnit) {

			int proximity_x = std::pow(((*sharedUnit)->getUnitGridPositionX() - rowFormationSpot->second.x), 2);
			int proximity_y = std::pow(((*sharedUnit)->getUnitGridPositionY() - rowFormationSpot->second.y), 2);

			int proximity = proximity_x + proximity_y;

			PotentialUnitLocationReference* unitLocation = new PotentialUnitLocationReference();
			unitLocation->unitRef = (*sharedUnit);
			
			unitLocation->locationRef = sharedLocation;
			unitLocation->distance = proximity;

			locationUnitMap.insert(std::pair<int, PotentialUnitLocationReference*>(proximity, unitLocation));
		}

		rowPermutationPriorityQueue[containerPosition].push_back(locationUnitMap);
	}

	/** Queue up every row unit by their proximity score. Then assign the potential location **/
	for (std::vector<std::vector<std::multimap<int, PotentialUnitLocationReference*>>>::iterator column = rowPermutationPriorityQueue.begin(); column != rowPermutationPriorityQueue.end(); ++column) {

		std::multimap<int, PotentialUnitLocationReference*> rowFormation;
		int rowSize = 0;

		for (std::vector<std::multimap<int, PotentialUnitLocationReference*>>::iterator row = column->begin(); row != column->end(); ++row) {
			rowSize = row->size();
			for (std::multimap<int, PotentialUnitLocationReference*>::iterator permutation = row->begin(); permutation != row->end(); ++permutation) {
				rowFormation.insert(*permutation);
			}
		}
		
		
		int unitsAssigned = 0;
		auto potentialMatch = rowFormation.begin();
		while (unitsAssigned < 3 && (allUnitsAssigned < totalUnits)) {
			SharedUnitReference* tempUnit = potentialMatch->second->unitRef;
			SharedLocationReference* tempLoc = potentialMatch->second->locationRef;
			if (tempUnit->available && tempLoc->available) {
				tempUnit->available = false;
				tempLoc->available = false;
				unitsAssigned++;
				allUnitsAssigned++;

				b2Vec2 finalPosition = GridUtils::b2NumericalCordFinder(tempLoc->potentialLocation);
				tempUnit->unit->b2FinalDestination = finalPosition;
			}

			++potentialMatch;
		}

	}


	//for (std::pair<Ogre::Vector2, std::multimap<int, PotentialUnitLocation*>> formationLocation : permutationPriorityQueue) {
	//	b2Vec2 finalPosition = GridUtils::b2NumericalCordFinder(formationLocation.second.begin()->second->potentialLocation);
	//	formationLocation.second.begin()->second->unit->b2FinalDestination = finalPosition;

	//	for (std::pair<Ogre::Vector2, std::multimap<int, PotentialUnitLocation*>> mapelement : permutationPriorityQueue) {
	//		for (std::pair<int, PotentialUnitLocation*>)
	//	}
	//}

	///** Map out the best location for each unit **/
	//for (std::map<Ogre::Vector2, std::multimap<int, PotentialUnitLocation*>>::iterator formationLocation = permutationPriorityQueue.begin(); formationLocation < permutationPriorityQueue.end(); ++formationLocation) {

	//}


}

/** Assign locations by row. The is the impl we are using. best compromise between accuracy and performance. **/
void UnitController::proximityLocationFormation(int width, int height, std::vector<Unit*> units, PathFinding* path) {
	int totalUnits = units.size();
	int rowSize = ceil(std::sqrt(totalUnits));
	int groupSize = 1;	

	int totalProximity = 0;
	std::multimap<int, Unit*> unitProximityOrder = std::multimap<int, Unit*>();
	std::vector<Ogre::Vector2> rowLocations = std::vector<Ogre::Vector2>();

	std::multimap<int, Ogre::Vector2>::iterator formationSpot = path->mappedFormation.begin();
	for (int i = 0; i < totalUnits; ++i) {
		rowLocations.push_back(Ogre::Vector2(formationSpot->second.x, formationSpot->second.y));
		formationSpot++;
	}
	int totalLocations = rowLocations.size();

	int currentSpot = 0;

	/** Find every units distance relationship with one another **/
	for (auto unit = units.begin(); unit < units.end(); ++unit) {
		int totalProximity = 0;

		for (auto rowLocation = rowLocations.begin(); rowLocation < rowLocations.end(); ++rowLocation) {
			int proximity_x = std::pow(((*unit)->currentPos.x - rowLocation->x), 2);
			int proximity_y = std::pow(((*unit)->currentPos.y - rowLocation->y), 2);
			int proximity = proximity_x + proximity_y;

			totalProximity += proximity;
		}
		unitProximityOrder.insert(std::pair<int, Unit*>(totalProximity, (*unit)));
	}

	/** preliminarily: track where we are in the row-column structure **/
	std::vector<std::vector<Unit*>*> unitRowColumnSort;
	std::vector<Unit*>* unitRowSort = new std::vector<Unit*>();
	unitRowColumnSort.push_back(unitRowSort);

	//TODO path->mappedFormation should be a double array not an <int, vector2> map
	std::vector<std::vector<Ogre::Vector2*>*> locationRowColumnSort;
	std::vector<Ogre::Vector2*>* locationRowSort = new std::vector<Ogre::Vector2*>();
	locationRowColumnSort.push_back(locationRowSort);

	int currentRow = 1;
	int containerPosition = 0;

	/** Schlep rowSize # of units from proximity order for organization purpose. **/
	auto rowFormationSpot = path->mappedFormation.begin();
	for (auto unitProximity = unitProximityOrder.begin(); unitProximity != unitProximityOrder.end(); ++unitProximity) {

		// Make sure we haven't moved up a row
		if (rowFormationSpot->first > currentRow) {
			currentRow = rowFormationSpot->first;

			std::vector<Unit*>* unitRowSortAddition = new std::vector<Unit*>();
			unitRowColumnSort.push_back(unitRowSortAddition);

			std::vector<Ogre::Vector2*>* locationRowSortAddition = new std::vector<Ogre::Vector2*>();
			locationRowColumnSort.push_back(locationRowSortAddition);

			containerPosition++;
		}

		unitRowColumnSort[containerPosition]->push_back(unitProximity->second);

		locationRowColumnSort[containerPosition]->push_back(&rowFormationSpot->second);
		rowFormationSpot++;
	}

	std::vector<std::vector<std::vector<Unit*>*>*> unitRowColumnCombinations;

	/** Run permutations on each row **/
	for (auto row = unitRowColumnSort.begin(); row < unitRowColumnSort.end(); ++row) {
		int unitRowSize = (*row)->size();
		std::stack<Unit*>* perm = new std::stack<Unit*>();
		std::vector<std::vector<Unit*>*>* unitRowCombinations = new std::vector<std::vector<Unit*>*>();

		permutations((*row), perm, unitRowSize, unitRowCombinations); // <-- not quite working

		unitRowColumnCombinations.push_back(unitRowCombinations);

		// cleanup
		delete(perm);
	}

	std::vector<std::multimap<int, std::vector<PotentialUnitLocation*>*>*> columnUnitLocMaps = std::vector<std::multimap<int, std::vector<PotentialUnitLocation*>*>*>();

	/** per column: Per row: find the best permutation **/
	std::vector<std::vector<Ogre::Vector2*>*>::iterator rowLocationSegment = locationRowColumnSort.begin();
	for (auto unitCombinations = unitRowColumnCombinations.begin(); unitCombinations < unitRowColumnCombinations.end(); ++unitCombinations) {

		std::multimap<int, std::vector<PotentialUnitLocation*>*>* unitLocMaps = new std::multimap<int, std::vector<PotentialUnitLocation*>*>();
		for (std::vector<std::vector<Unit*>*>::iterator unitPermutation = (*unitCombinations)->begin(); unitPermutation != (*unitCombinations)->end(); ++unitPermutation) {

			std::vector<PotentialUnitLocation*>* unitFormation = new std::vector<PotentialUnitLocation*>();
			std::vector<Ogre::Vector2*>::iterator position = (*rowLocationSegment)->begin();
			int maxDistance = 0;
			int minDistance = 0;
			int totalProximity = 0;
			for (std::vector<Unit*>::iterator unitsInPermutation = (*unitPermutation)->begin(); unitsInPermutation != (*unitPermutation)->end(); ++unitsInPermutation) {
				int proximity_x = std::pow(((*unitsInPermutation)->currentPos.x - (*position)->x), 2);
				int proximity_y = std::pow(((*unitsInPermutation)->currentPos.y - (*position)->y), 2);

				int proximity = proximity_x + proximity_y;

				PotentialUnitLocation* unitLocation = new PotentialUnitLocation();
				unitLocation->unit = (*unitsInPermutation);
				unitLocation->potentialLocation = *(*position);
				unitLocation->distance = proximity;

				unitFormation->push_back(unitLocation);
				totalProximity += proximity;

				position++;
			}
			unitLocMaps->insert(std::pair<int, std::vector<PotentialUnitLocation*>*>(totalProximity, unitFormation));
		}
		columnUnitLocMaps.push_back(unitLocMaps);

		rowLocationSegment++;
	}

	/* Assign destinations to units */
	for (auto unitLocMaps = columnUnitLocMaps.begin(); unitLocMaps < columnUnitLocMaps.end(); ++unitLocMaps) {
		for (std::vector<PotentialUnitLocation*>::iterator shortestLocations = (*unitLocMaps)->begin()->second->begin(); shortestLocations < (*unitLocMaps)->begin()->second->end(); shortestLocations++) {
			b2Vec2 finalPosition = GridUtils::b2NumericalCordFinder((*shortestLocations)->potentialLocation);
			(*shortestLocations)->unit->b2FinalDestination = finalPosition;
			//(*shortestLocations)->unit->mState = (*shortestLocations)->unit->mPreviousState;
		}
	}
	
}

/** by row impl. Efficient but not as good as the shotgun approach **/
void UnitController::fullLocationFormation(int width, int height, std::vector<Unit*> units, PathFinding* path) {

	proximityLocationFormation(width, height, units, path);
	//priorityQueueFormation(width, height, units, path);
	return;

	int rowSize = 3;
	int groupSize = 1;
	int iterations = (units.size() + (rowSize - 1)) / rowSize;
	int totalUnits = units.size();

	int totalProximity = 0;
	std::multimap<int, std::vector<PotentialUnitLocation*>*> unitLocMaps = std::multimap<int, std::vector<PotentialUnitLocation*>*>();
	std::vector<Ogre::Vector2> rowLocations = std::vector<Ogre::Vector2>();

	std::multimap<int, Ogre::Vector2>::iterator formationSpot = path->mappedFormation.begin();
	for (int i = 0; i < totalUnits; ++i) {
		rowLocations.push_back(Ogre::Vector2(formationSpot->second.x, formationSpot->second.y));
		formationSpot++;
	}
	int totalLocations = rowLocations.size();


	// Find every possible array combination of Units
	std::vector<std::vector<Unit*>*>* unitCombinations = new std::vector<std::vector<Unit*>*>();
	std::vector<Unit*> sixUnits;
	for (std::vector<Unit*>::iterator it = units.begin(); it < units.end(); ++it) {
		/*if (sixUnits.size() < 6)*/
			sixUnits.push_back((*it));
	}
	std::stack<Unit*>* perm = new std::stack<Unit*>();
	permutations(&sixUnits, perm, sixUnits.size(), unitCombinations);
	//permutations(&units, perm, units.size(), unitCombinations);

	for (std::vector<std::vector<Unit*>*>::iterator unitPermutation = unitCombinations->begin(); unitPermutation != unitCombinations->end(); ++unitPermutation) {

		std::vector<PotentialUnitLocation*>* unitFormation = new std::vector<PotentialUnitLocation*>();
		std::vector<Ogre::Vector2>::iterator position = rowLocations.begin();
		int maxDistance = 0;
		int minDistance = 0;
		int totalProximity = 0;
		for (std::vector<Unit*>::iterator unitsInPermutation = (*unitPermutation)->begin(); unitsInPermutation != (*unitPermutation)->end(); ++unitsInPermutation) {
			int proximity_x = std::pow(((*unitsInPermutation)->currentPos.x - position->x), 2);
			int proximity_y = std::pow(((*unitsInPermutation)->currentPos.y - position->y), 2);

			int proximity = proximity_x + proximity_y;

			PotentialUnitLocation* unitLocation = new PotentialUnitLocation();
			unitLocation->unit = (*unitsInPermutation);
			unitLocation->potentialLocation = (*position);
			unitLocation->distance = proximity;

			unitFormation->push_back(unitLocation);
			totalProximity += proximity;

			position++;
		}
		unitLocMaps.insert(std::pair<int, std::vector<PotentialUnitLocation*>*>(totalProximity, unitFormation));

	}

	/* Assign destinations to units */
	for (std::vector<PotentialUnitLocation*>::iterator shortestLocations = unitLocMaps.begin()->second->begin(); shortestLocations < unitLocMaps.begin()->second->end(); shortestLocations++) {
		b2Vec2 finalPosition = GridUtils::b2NumericalCordFinder((*shortestLocations)->potentialLocation);
		(*shortestLocations)->unit->b2FinalDestination = finalPosition;

		//(*shortestLocations)->unit->finalDestination = Ogre::Vector3(finalPosition.x, 0, finalPosition.y);
		//(*shortestLocations)->unit->debugPos1 = (*shortestLocations)->potentialLocation; //TODO remove this
	}

	/*for (std::vector<std::vector<Unit*>*>::iterator unitPermutation = unitCombinations->begin(); unitPermutation != unitCombinations->end(); ++unitPermutation) {
		for (std::vector<Unit*>::iterator unitsInPermutation = (*unitPermutation)->begin(); unitsInPermutation != (*unitPermutation)->end(); ++unitsInPermutation) {
			delete(*unitsInPermutation);

		}
		delete(*unitPermutation);
	}
	delete(unitCombinations);*/
}

/*** Shotgun Impl. This works but past 7 units its not performant ***/
//void GameObjectManager::fullLocationFormation(int width, int height, std::vector<Unit*> units, PathFinding* path) {
//	int rowSize = 3;
//	int groupSize = 1;
//	int iterations = (units.size() + (rowSize - 1)) / rowSize;
//	int totalUnits = units.size();
//
//	int totalProximity = 0;
//	std::multimap<int, std::vector<PotentialUnitLocation*>*> unitLocMaps = std::multimap<int, std::vector<PotentialUnitLocation*>*>();
//	std::vector<Ogre::Vector2> rowLocations = std::vector<Ogre::Vector2>();
//
//	std::multimap<int, Ogre::Vector2>::iterator formationSpot = path->mappedFormation.begin();
//	for (int i = 0; i < totalUnits; ++i) {
//		rowLocations.push_back(Ogre::Vector2(formationSpot->second.x, formationSpot->second.y));
//		formationSpot++;
//	}
//	int totalLocations = rowLocations.size();
//
//	// Find every possible array combination of Units
//	std::vector<std::vector<Unit*>*>* unitCombinations = new std::vector<std::vector<Unit*>*>();
//	std::stack<Unit*>* perm = new std::stack<Unit*>();
//	permutations(&units, perm, units.size(), unitCombinations);
//
//
//	for (std::vector<std::vector<Unit*>*>::iterator unitPermutation = unitCombinations->begin(); unitPermutation != unitCombinations->end(); ++unitPermutation) {
//
//		std::vector<PotentialUnitLocation*>* unitFormation = new std::vector<PotentialUnitLocation*>();
//		std::vector<Ogre::Vector2>::iterator position = rowLocations.begin();
//		int maxDistance = 0;
//		int minDistance = 0;
//		int totalProximity = 0;
//		for (std::vector<Unit*>::iterator unitsInPermutation = (*unitPermutation)->begin(); unitsInPermutation != (*unitPermutation)->end(); ++unitsInPermutation) {
//			int proximity_x = std::pow(((*unitsInPermutation)->currentPos.x - position->x), 2);
//			int proximity_y = std::pow(((*unitsInPermutation)->currentPos.y - position->y), 2);
//
//			int proximity = proximity_x + proximity_y;
//
//			PotentialUnitLocation * unitLocation = new PotentialUnitLocation();
//			unitLocation->unit = (*unitsInPermutation);
//			unitLocation->potentialLocation = (*position);
//			unitLocation->distance = proximity;
//
//			unitFormation->push_back(unitLocation);
//			totalProximity += proximity;
//
//			position++;
//		}
//		unitLocMaps.insert(std::pair<int, std::vector<PotentialUnitLocation*>*>(totalProximity, unitFormation));
//
//	}
//
//		/* Assign destinations to units */
//	for (std::vector<PotentialUnitLocation*>::iterator shortestLocations = unitLocMaps.begin()->second->begin(); shortestLocations < unitLocMaps.begin()->second->end(); shortestLocations++) {
//		b2Vec2 finalPosition = GridUtils::b2NumericalCordFinder((*shortestLocations)->potentialLocation);
//		(*shortestLocations)->unit->b2FinalDestination = finalPosition;
//
//		//(*shortestLocations)->unit->finalDestination = Ogre::Vector3(finalPosition.x, 0, finalPosition.y);
//		//(*shortestLocations)->unit->debugPos1 = (*shortestLocations)->potentialLocation; //TODO remove this
//	}
//
//}

/*** Original Impl. Doesn't work well since it doesn't find the lowest score ***/
//void GameObjectManager::assignUnitToFormationLocation(int width, int height, std::vector<Unit*> units, PathFinding* path) {
//	std::multimap<int, Unit*> unitProximities = std::multimap<int, Unit*>();
//
//	int rowSize = 3;
//	int groupSize = 1;
//	int iterations = (units.size() + (rowSize - 1)) / rowSize;
//	int totalUnits = units.size();
//
//	for (std::vector<Unit*>::iterator unit = units.begin(); unit != units.end(); ++unit) {
//		int proximity_x = std::pow(((*unit)->getB2DPosition().x - width), 2);
//		int proximity_y = std::pow(((*unit)->getB2DPosition().y - height), 2);
//		int proximity = proximity_x + proximity_y;
//
//		unitProximities.insert(std::pair<int, Unit*>(proximity, (*unit)));
//	}
//
//
//	/* Find how many units we need for the given row */
//	std::multimap<int, Ogre::Vector2>::iterator position = path->mappedFormation.begin();
//	std::multimap<int, Ogre::Vector2>::iterator topographer = path->mappedFormation.begin();
//	int rows = 0;
//	int columnCount = 1;
//	while (rows < iterations) {
//
//		int rowColumnKey = topographer->first;
//		int numLocations = 0;
//		while (topographer->first == rowColumnKey) {
//			numLocations++;
//			topographer++;
//		}
//
//		int numUnits = 0;
//		if (totalUnits > numLocations) {
//			numUnits = numLocations;
//			totalUnits -= numLocations;
//		}
//		else {
//			numUnits = totalUnits;
//		}
//
//
//		/* Grab a numUnits amount of unit proximities */
//		std::vector<Unit*> rowUnits = std::vector<Unit*>();
//		std::multimap<int, Unit*>::iterator unitProximity = unitProximities.begin();
//		unitProximity->second->debugInt1 = numUnits;
//		for (int i = 0; i < numUnits; i++) {
//			rowUnits.push_back(unitProximity->second);
//			unitProximities.erase(unitProximity);
//			unitProximity++;
//		}
//
//
//		/* loop through the units and map out every possible combination in the row */
//		std::vector<Ogre::Vector2> rowLocations = std::vector<Ogre::Vector2>();
//		while (position->first == columnCount) {
//			rowLocations.push_back(position->second);
//			position++;
//		}
//		columnCount++;
//
//		std::multimap<int, std::vector<PotentialUnitLocation*>*> unitLocMaps = std::multimap<int, std::vector<PotentialUnitLocation*>*>();
//		for (int x = 0; x < numUnits; x++) {
//
//			int unitCount = 0;
//			int locationCount = 0;
//
//			for (int y = 0; y < numUnits; y++) {
//
//				int totalProximity = 0;
//				std::vector<PotentialUnitLocation*>* unitsForRow = new std::vector<PotentialUnitLocation*>();
//				std::vector<Ogre::Vector2>::iterator location = rowLocations.begin();
//				std::vector<Unit*>::iterator unit = rowUnits.begin();
//
//				for (int z = 0; z < numUnits; z++) {
//					if (location == rowLocations.end()) {
//						break;
//					}
//					int proximity_x = std::pow(((*unit)->currentPos.x - location->x), 2);
//					int proximity_y = std::pow(((*unit)->currentPos.y - location->y), 2);
//
//					//int proximity_x = std::pow((location->x - (*unit)->currentPos.x), 2);
//					//(*unit)->debugInt1 = proximity_x;
//					//int proximity_y = std::pow((location->y - (*unit)->currentPos.y), 2);
//					int proximity = proximity_x + proximity_y;
//
//					PotentialUnitLocation * unitLocation = new PotentialUnitLocation();
//					unitLocation->unit = (*unit);
//					unitLocation->potentialLocation = (*location);
//					unitLocation->distance = proximity;
//
//					unitsForRow->push_back(unitLocation);
//					totalProximity += proximity;
//
//					location++;
//					unit++;
//				}
//
//				unitLocMaps.insert(std::pair<int, std::vector<PotentialUnitLocation*>*>(totalProximity, unitsForRow));
//				std::rotate(rowUnits.begin(), rowUnits.end() - 1, rowUnits.end());
//			}
//			std::rotate(rowLocations.begin(), rowLocations.end() - 1, rowLocations.end());
//		}
//
//
//		for (std::vector<PotentialUnitLocation*>::iterator shortestLocations = unitLocMaps.begin()->second->begin(); shortestLocations < unitLocMaps.begin()->second->end(); shortestLocations++) {
//			b2Vec2 finalPosition = GridUtils::b2NumericalCordFinder((*shortestLocations)->potentialLocation);
//			(*shortestLocations)->unit->b2FinalDestination = finalPosition;
//
//			(*shortestLocations)->unit->finalDestination = Ogre::Vector3(finalPosition.x, 0, finalPosition.y);
//			(*shortestLocations)->unit->debugPos1 = (*shortestLocations)->potentialLocation; //TODO remove this
//		}
//
//		//for (std::vector<PotentialUnitLocation*>::iterator shortestLocations = unitLocMaps.begin()->second->begin(); shortestLocations < unitLocMaps.begin()->second->end(); shortestLocations++) {
//		//	b2Vec2 finalPosition = GridUtils::b2NumericalCordFinder((*shortestLocations)->potentialLocation);
//		//	(*shortestLocations)->unit->b2FinalDestination = finalPosition;
//
//		//	(*shortestLocations)->unit->finalDestination = Ogre::Vector3(finalPosition.x, 0, finalPosition.y);
//		//	(*shortestLocations)->unit->debugPos1 = (*shortestLocations)->potentialLocation; //TODO remove this
//		//}
//		rows++;
//	}
//}
