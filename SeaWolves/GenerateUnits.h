#pragma once

#include "Ogre.h"

#include "Unit.h"
#include "Constants.h"

class GenerateUnits
{
public:
	GenerateUnits();
	~GenerateUnits();

	static void generateOneBronze(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit>* units);

	static void generateFourBronze(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy);

	static void generateEightBronze(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit>* units);

	static void generateOneSky(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy);

	static void generateFourSky(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit*>* units, std::map<Ogre::String, Unit*>* playerArmy);

	static void generateEightSky(Ogre::SceneManager* mScnMgr, std::map<Ogre::String, Unit>* units);
};

