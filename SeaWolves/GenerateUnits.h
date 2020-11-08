#pragma once

#include "Ogre.h"

#include "UnitController.h"
#include "Unit.h"
#include "Soldier.h"
#include "Archer.h"
#include "Wizard.h"

#include "Constants.h"
#include "Box2D\Box2D.h"
#include "GridUtils.h"

class GenerateUnits
{
public:
	GenerateUnits();
	~GenerateUnits();

	static void generateOneBronze(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy, b2World* world, std::vector<GridSquare*>* impassableTerrain, UnitController* unitController);

	static void generateFourBronze(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy, b2World* world, std::vector<GridSquare*>* impassableTerrain, UnitController* unitController);

	static void generateEightBronze(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy, b2World* world, std::vector<GridSquare*>* impassableTerrain, UnitController* unitController);

	static void generateOneSky(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy, b2World* world, std::vector<GridSquare*>* impassableTerrain, UnitController* unitController);

	static void generateFourSky(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy, b2World* world, std::vector<GridSquare*>* impassableTerrain, UnitController* unitController);

	static void generateEightSky(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy, b2World* world, std::vector<GridSquare*>* impassableTerrain, UnitController* unitController);
};

