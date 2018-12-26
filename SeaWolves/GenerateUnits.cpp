#include "GenerateUnits.h"



GenerateUnits::GenerateUnits()
{
}


GenerateUnits::~GenerateUnits()
{
}


void GenerateUnits::generateOneBronze(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit>* units) {
	int unitNumber = 0;
	Unit* unit;
	Constants constants;
	Ogre::String soldierBronze = constants.bronzeSoldierMesh;
	Ogre::String archerBronze = constants.bronzeArcherMesh;
	Ogre::String wizardBronze = constants.bronzeWizardMesh;

	Ogre::String unit1 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 45.0f), unit1, soldierBronze, 1);
	units->insert(std::make_pair(unit1, *unit));
}

void GenerateUnits::generateFourBronze(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy) {
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
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 150.0f), unit1, archerBronze, 1);
	unit->currentPos = Ogre::Vector2(0, 2);
	unit->attackRange = 200;
	units->insert(std::make_pair(unit1, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit1, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 100.0f), unit2, soldierBronze, 2);
	unit->currentPos = Ogre::Vector2(0, 1);
	unit->attackRange = 50;
	units->insert(std::make_pair(unit2, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit2, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(65.0f, 0.0f, 50.0f), unit3, soldierBronze, 3);
	unit->currentPos = Ogre::Vector2(1, 0);
	unit->attackRange = 50;
	units->insert(std::make_pair(unit3, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit3, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(65.0f, 0.0f, 100.0f), unit4, wizardBronze, 4);
	unit->currentPos = Ogre::Vector2(1, 1);
	unit->attackRange = 150;
	units->insert(std::make_pair(unit4, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit4, unit));
}

void GenerateUnits::generateEightBronze(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit>* units) {
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

	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 45.0f), unit1, soldierBronze, 1);
	unit->currentPos = Ogre::Vector2(0, 0);
	units->insert(std::make_pair(unit1, *unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 100.0f), unit2, soldierBronze, 2);
	unit->currentPos = Ogre::Vector2(0, 1);
	units->insert(std::make_pair(unit2, *unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(65.0f, 0.0f, 50.0f), unit3, soldierBronze, 3);
	unit->currentPos = Ogre::Vector2(1, 0);
	units->insert(std::make_pair(unit3, *unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(65.0f, 0.0f, 100.0f), unit4, wizardBronze, 4);
	unit->currentPos = Ogre::Vector2(1, 1);
	units->insert(std::make_pair(unit4, *unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 150.0f), unit5, archerBronze, 5);
	unit->currentPos = Ogre::Vector2(0, 2);
	units->insert(std::make_pair(unit5, *unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 200.0f), unit6, archerBronze, 6);
	unit->currentPos = Ogre::Vector2(0, 2);
	units->insert(std::make_pair(unit6, *unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(65.0f, 0.0f, 150.0f), unit7, archerBronze, 7);
	unit->currentPos = Ogre::Vector2(1, 2);
	units->insert(std::make_pair(unit7, *unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(65.0f, 0.0f, 200.0f), unit8, wizardBronze, 8);
	unit->currentPos = Ogre::Vector2(1, 2);
	units->insert(std::make_pair(unit8, *unit));
}

void GenerateUnits::generateOneSky(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy) {
	int unitNumber = 100;
	Unit* unit;
	Constants constants;
	Ogre::String soldierSky = constants.skySoldierMesh;
	Ogre::String archerSky = constants.skyArcherMesh;
	Ogre::String wizardSky = constants.skyWizardMesh;

	Ogre::String unit1 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	unit = new Unit(mScnMgr, Ogre::Vector3(300.0f, 0.0f, 200.0f), unit1, soldierSky, 101);
	unit->currentPos = Ogre::Vector2(2, 3);
	unit->attackRange = 50;
	units->insert(std::make_pair(unit1, unit));
	playerArmy->insert(std::make_pair(unit1, unit));
}

void GenerateUnits::generateFourSky(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy) {
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
	unit = new Unit(mScnMgr, Ogre::Vector3(300.0f, 0.0f, 200.0f), unit1, soldierSky, 101);
	unit->currentPos = Ogre::Vector2(11, 11);
	units->insert(std::make_pair(unit1, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit1, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(300.0f, 0.0f, 250.0f), unit2, soldierSky, 102);
	unit->currentPos = Ogre::Vector2(11, 12);
	units->insert(std::make_pair(unit2, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit2, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(300.0f, 0.0f, 300.0f), unit3, soldierSky, 103);
	unit->currentPos = Ogre::Vector2(12, 11);
	units->insert(std::make_pair(unit3, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit3, unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(300.0f, 0.0f, 350.0f), unit4, soldierSky, 104);
	unit->currentPos = Ogre::Vector2(12, 12);
	units->insert(std::make_pair(unit4, unit));
	playerArmy->insert(std::pair<Ogre::String, Unit*>(unit4, unit));
}

void GenerateUnits::generateEightSky(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit>* units) {
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
	Ogre::String unit5 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit6 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit7 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;
	Ogre::String unit8 = "UnitNode" + Ogre::StringConverter::toString(unitNumber);
	unitNumber++;

	unit = new Unit(mScnMgr, Ogre::Vector3(315.0f, 0.0f, 45.0f), unit1, soldierSky, 1);
	unit->currentPos = Ogre::Vector2(0, 0);
	units->insert(std::make_pair(unit1, *unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(315.0f, 0.0f, 100.0f), unit2, soldierSky, 2);
	unit->currentPos = Ogre::Vector2(0, 1);
	units->insert(std::make_pair(unit2, *unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(365.0f, 0.0f, 50.0f), unit3, soldierSky, 3);
	unit->currentPos = Ogre::Vector2(1, 0);
	units->insert(std::make_pair(unit3, *unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(365.0f, 0.0f, 100.0f), unit4, wizardSky, 4);
	unit->currentPos = Ogre::Vector2(1, 1);
	units->insert(std::make_pair(unit4, *unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(315.0f, 0.0f, 150.0f), unit5, archerSky, 5);
	unit->currentPos = Ogre::Vector2(0, 0);
	units->insert(std::make_pair(unit5, *unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(315.0f, 0.0f, 200.0f), unit6, archerSky, 6);
	unit->currentPos = Ogre::Vector2(0, 1);
	units->insert(std::make_pair(unit6, *unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(365.0f, 0.0f, 150.0f), unit7, archerSky, 7);
	unit->currentPos = Ogre::Vector2(1, 0);
	units->insert(std::make_pair(unit7, *unit));
	unit = new Unit(mScnMgr, Ogre::Vector3(365.0f, 0.0f, 200.0f), unit8, wizardSky, 8);
	unit->currentPos = Ogre::Vector2(1, 1);
	units->insert(std::make_pair(unit8, *unit));
}