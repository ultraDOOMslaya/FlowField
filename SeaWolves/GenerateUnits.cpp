#include "GenerateUnits.h"



GenerateUnits::GenerateUnits()
{
}


GenerateUnits::~GenerateUnits()
{
}


void GenerateUnits::generateOneBronze(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy, b2World* world, std::vector<GridSquare*>*   impassableTerrain, UnitController* unitController) {
	int unitNumber = 0;
	Unit* unit;
	Constants constants;
	Ogre::String soldierBronze = constants.bronzeSoldierMesh;
	Ogre::String archerBronze = constants.bronzeArcherMesh;
	Ogre::String wizardBronze = constants.bronzeWizardMesh;

	Ogre::String unit2 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	
	unit = new Soldier(mScnMgr, GridUtils::numericalCordFinder(0, 1), unit2, soldierBronze, "HeavyArmor", 2, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(0, 1);
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit2, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit2, unit));
}

void GenerateUnits::generatUnit(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy, b2World* world, std::vector<GridSquare*>* impassableTerrain, UnitController* unitController, int unitId, Ogre::Vector2 spawnPoint) {
	int unitNumber = unitId;
	Unit* unit;
	Constants constants;
	Ogre::String soldierBronze = constants.bronzeSoldierMesh;
	Ogre::String archerBronze = constants.bronzeArcherMesh;
	Ogre::String wizardBronze = constants.bronzeWizardMesh;

	Ogre::String unit2 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;

	unit = new Soldier(mScnMgr, GridUtils::numericalCordFinder(spawnPoint.x, spawnPoint.y), unit2, soldierBronze, "HeavyArmor", unitId, world, impassableTerrain, unitController);
	unit->currentPos = spawnPoint;
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit2, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit2, unit));
}

void GenerateUnits::generateFourBronze(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy, b2World* world, std::vector<GridSquare*>* impassableTerrain, UnitController* unitController) {
	int unitNumber = 0;
	Unit* unit;
	Constants constants;
	Ogre::String soldierBronze = constants.bronzeSoldierMesh;
	Ogre::String archerBronze = constants.bronzeArcherMesh;
	Ogre::String wizardBronze = constants.bronzeWizardMesh;

	Ogre::String unit1 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit2 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit3 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit4 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 150.0f), unit1, archerBronze, "LightArmor", 1, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(0, 2);
	unit->attackRange = 200;
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit1, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit1, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 100.0f), unit2, soldierBronze, "HeavyArmor", 2, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(0, 1);
	unit->attackRange = 50;
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit2, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit2, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(65.0f, 0.0f, 50.0f), unit3, soldierBronze, "HeavyArmor", 3, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(1, 0);
	unit->attackRange = 50;
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit3, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit3, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(65.0f, 0.0f, 100.0f), unit4, wizardBronze, "LightArmor", 4, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(1, 1);
	unit->attackRange = 150;
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit4, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit4, unit));
}

void GenerateUnits::generateEightBronze(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy, b2World* world, std::vector<GridSquare*>* impassableTerrain, UnitController* unitController) {
	int unitNumber = 0;
	Unit* unit;
	Constants constants;
	Ogre::String soldierBronze = constants.bronzeSoldierMesh;
	Ogre::String archerBronze = constants.bronzeArcherMesh;
	Ogre::String wizardBronze = constants.bronzeWizardMesh;

	Ogre::String unit1 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit2 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit3 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit4 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit5 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit6 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit7 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit8 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit9 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	//Ogre::String unit10 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	//unitNumber++;

	unit = new Soldier(mScnMgr, GridUtils::numericalCordFinder(0, 1), unit2, soldierBronze, "HeavyArmor", 2, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(0, 1);
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit2, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit2, unit));
	unit = new Soldier(mScnMgr, GridUtils::numericalCordFinder(1, 0), unit3, soldierBronze, "HeavyArmor", 3, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(1, 0);
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit3, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit3, unit));
	unit = new Wizard(mScnMgr, GridUtils::numericalCordFinder(1, 1), unit4, wizardBronze, "Caster", 4, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(1, 1);
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit4, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit4, unit));
	unit = new Archer(mScnMgr, GridUtils::numericalCordFinder(0, 2), unit5, archerBronze, "Fletcher", 5, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(0, 2);
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit5, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit5, unit));
	unit = new Archer(mScnMgr, GridUtils::numericalCordFinder(1, 2), unit6, archerBronze, "Fletcher", 6, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(1, 2);
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit6, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit6, unit));
	unit = new Archer(mScnMgr, GridUtils::numericalCordFinder(0, 3), unit7, archerBronze, "Fletcher", 7, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(0, 3);
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit7, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit7, unit));
	unit = new Wizard(mScnMgr, GridUtils::numericalCordFinder(1, 3), unit8, wizardBronze, "Caster", 8, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(1, 3);
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit8, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit8, unit));
	unit = new Soldier(mScnMgr, GridUtils::numericalCordFinder(0, 4), unit1, soldierBronze, "HeavyArmor", 1, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(0, 4);
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit1, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit1, unit));
	unit = new Soldier(mScnMgr, GridUtils::numericalCordFinder(1, 5), unit9, soldierBronze, "HeavyArmor", 9, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(1, 5);
	unit->mPlayerId = 1;
	units->insert(std::make_pair(unit9, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit9, unit));
	/*unit = new Unit(mScnMgr, GridUtils::numericalCordFinder(1, 6), unit10, soldierBronze, "HeavyArmor", 10, world);
	unit->currentPos = Ogre::Vector2(1, 6);
	units->insert(std::make_pair(unit10, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit10, unit));*/
}

void GenerateUnits::generateOneSky(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy, b2World* world, std::vector<GridSquare*>*   impassableTerrain, UnitController* unitController) {
	int unitNumber = 100;
	Unit* unit;
	Constants constants;
	Ogre::String soldierSky = constants.skySoldierMesh;
	Ogre::String archerSky = constants.skyArcherMesh;
	Ogre::String wizardSky = constants.skyWizardMesh;

	Ogre::String unit1 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	unit = new Soldier(mScnMgr, GridUtils::numericalCordFinder(9, 4), unit1, soldierSky, "HeavyArmor", 101, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(9, 4);
	unit->attackRange = 50;
	unit->mPlayerId = 2;
	units->insert(std::make_pair(unit1, unit));
	playerArmy->insert(std::make_pair(unit1, unit));
}

void GenerateUnits::generateFourSky(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy, b2World* world, std::vector<GridSquare*>*   impassableTerrain, UnitController* unitController) {
	int unitNumber = 100;
	Unit* unit;
	Constants constants;
	Ogre::String soldierSky = constants.skySoldierMesh;
	Ogre::String archerSky = constants.skyArcherMesh;
	Ogre::String wizardSky = constants.skyWizardMesh;

	Ogre::String unit1 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit2 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit3 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit4 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	unit = new Soldier(mScnMgr, GridUtils::numericalCordFinder(6, 4), unit1, soldierSky, "HeavyArmor", 101, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(6, 4);
	unit->mPlayerId = 2;
	units->insert(std::make_pair(unit1, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit1, unit));
	unit = new Soldier(mScnMgr, GridUtils::numericalCordFinder(6, 5), unit2, soldierSky, "HeavyArmor", 102, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(6, 5);
	unit->mPlayerId = 2;
	units->insert(std::make_pair(unit2, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit2, unit));
	unit = new Soldier(mScnMgr, GridUtils::numericalCordFinder(7, 5), unit3, soldierSky, "LightArmor", 103, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(7, 5);
	unit->mPlayerId = 2;
	units->insert(std::make_pair(unit3, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit3, unit));
	unit = new Soldier(mScnMgr, GridUtils::numericalCordFinder(7, 6), unit4, soldierSky, "LightArmor", 104, world, impassableTerrain, unitController);
	unit->currentPos = Ogre::Vector2(7, 6);
	unit->mPlayerId = 2;
	units->insert(std::make_pair(unit4, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit4, unit));
}

void GenerateUnits::generateEightSky(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy, b2World* world, std::vector<GridSquare*>*   impassableTerrain, UnitController* unitController) {
	/*int unitNumber = 100;
	Unit* unit;
	Constants constants;
	Ogre::String soldierSky = constants.skySoldierMesh;
	Ogre::String archerSky = constants.skyArcherMesh;
	Ogre::String wizardSky = constants.skyWizardMesh;

	Ogre::String unit1 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit2 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit3 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit4 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit5 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit6 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit7 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit8 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;

	unit = new Unit(mScnMgr, Ogre::Vector3(315.0f, 0.0f, 45.0f), unit1, soldierSky, 1, world);
	unit->currentPos = Ogre::Vector2(0, 0);
	units->insert(std::make_pair(unit1, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit1, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(315.0f, 0.0f, 100.0f), unit2, soldierSky, 2, world);
	unit->currentPos = Ogre::Vector2(0, 1);
	units->insert(std::make_pair(unit2, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit2, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(365.0f, 0.0f, 50.0f), unit3, soldierSky, 3, world);
	unit->currentPos = Ogre::Vector2(1, 0);
	units->insert(std::make_pair(unit3, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit3, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(365.0f, 0.0f, 100.0f), unit4, wizardSky, 4, world);
	unit->currentPos = Ogre::Vector2(1, 1);
	units->insert(std::make_pair(unit4, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit4, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(315.0f, 0.0f, 150.0f), unit5, archerSky, 5, world);
	unit->currentPos = Ogre::Vector2(0, 0);
	units->insert(std::make_pair(unit5, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit5, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(315.0f, 0.0f, 200.0f), unit6, archerSky, 6, world);
	unit->currentPos = Ogre::Vector2(0, 1);
	units->insert(std::make_pair(unit6, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit6, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(365.0f, 0.0f, 150.0f), unit7, archerSky, 7, world);
	unit->currentPos = Ogre::Vector2(1, 0);
	units->insert(std::make_pair(unit7, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit7, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(365.0f, 0.0f, 200.0f), unit8, wizardSky, 8, world);
	unit->currentPos = Ogre::Vector2(1, 1);
	units->insert(std::make_pair(unit8, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit8, unit));*/
}

void GenerateUnits::generateTrees(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, NaturalResource*>* natResources, b2World* world, std::vector<GridSquare*>* impassableTerrain) {
	Ogre::Entity* treeEntity = mScnMgr->createEntity("OakTree.mesh");

	Ogre::Vector3 tree1Pos = GridUtils::numericalCordFinder(6, 7);
	tree1Pos.y += Constants::unitBaseHeight;
	tree1Pos.x += 40;
	tree1Pos.z += 40;
	ForestTree* tree1 = new ForestTree(mScnMgr, tree1Pos, "oak1", "OakTree.mesh", world, impassableTerrain);
	natResources->insert(std::pair<Ogre::String, NaturalResource*>("oak1", tree1));

	Ogre::Vector3 tree2Pos = GridUtils::numericalCordFinder(11, 5);
	tree2Pos.y += Constants::unitBaseHeight;
	tree2Pos.x += 40;
	tree2Pos.z += 40;
	ForestTree* tree2 = new ForestTree(mScnMgr, tree2Pos, "poplar1", "PoplarTree.mesh", world, impassableTerrain);
	natResources->insert(std::pair<Ogre::String, NaturalResource*>("poplar1", tree2));

	Ogre::Vector3 tree3Pos = GridUtils::numericalCordFinder(12, 5);
	tree3Pos.y += Constants::unitBaseHeight;
	tree3Pos.x += 40;
	tree3Pos.z += 40;
	ForestTree* tree3 = new ForestTree(mScnMgr, tree3Pos, "oak2", "OakTree.mesh", world, impassableTerrain);
	natResources->insert(std::pair<Ogre::String, NaturalResource*>("oak2", tree3));

	Ogre::Vector3 tree4Pos = GridUtils::numericalCordFinder(13, 8);
	tree4Pos.y += Constants::unitBaseHeight;
	tree4Pos.x += 40;
	tree4Pos.z += 40;
	ForestTree* tree4 = new ForestTree(mScnMgr, tree4Pos, "poplar2", "PoplarTree.mesh", world, impassableTerrain);
	natResources->insert(std::pair<Ogre::String, NaturalResource*>("poplar2", tree4));
}

void GenerateUnits::generateBuildings(Ogre::SceneManager* gameSceneManager, Ogre::Vector3 buildingPosition, int buildingId, Ogre::String meshName, b2World* world, std::vector<GridSquare*>* impassableTerrain, int ticksToCompletion, std::map<Ogre::String, Building*>* playerBuildings, std::map<Ogre::String, Building*>* buildings) {
	buildingPosition.y += Constants::unitBaseHeight;
	Barracks* redRax = new Barracks(gameSceneManager, buildingPosition, buildingId, "RedBrickShack.mesh", world, impassableTerrain, 0);
	buildings->insert(std::pair<Ogre::String, Building*>(redRax->getName(), redRax));
	playerBuildings->insert(std::pair<Ogre::String, Building*>(redRax->getName(), redRax));
}

Building* GenerateUnits::generateBuilding(Ogre::SceneManager* gameSceneManager, Ogre::Vector3 buildingPosition, int buildingId, Ogre::String meshName, b2World* world, std::vector<GridSquare*>* impassableTerrain, int ticksToCompletion, std::map<Ogre::String, Building*>* playerBuildings, std::map<Ogre::String, Building*>* buildings) {
	Barracks* redRax = new Barracks(gameSceneManager, buildingPosition, buildingId, "RedBrickShack.mesh", world, impassableTerrain, ticksToCompletion);
	buildings->insert(std::pair<Ogre::String, Building*>(redRax->getName(), redRax));
	playerBuildings->insert(std::pair<Ogre::String, Building*>(redRax->getName(), redRax));
	return redRax;
}