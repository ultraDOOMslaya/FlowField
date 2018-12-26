#pragma once
#ifndef __Constants_h_
#define __Constants_h_

#include <Ogre.h>

class Constants
{
public:
	Constants();
	~Constants();

	const static int			gridWidth = 15;
	const static int			gridHeight = 15;
	const static int			edgeLength = 50; //was 50
	//const static int			edgeLength = 65; //was 50
	//const static int			dimension = gridWidth;	//TODO: not a good standard... should be fixed
	const static int			dimension = 15;	//TODO: not a good standard... should be fixed
	const static int			WALL = std::numeric_limits<int>::max();
	const static int			terrainQueryMask = 1;
	const static int			unitQueryMask = 2;
	const static int			awareRadius = 500;

	const Ogre::String			robot = "robot.mesh";
	Ogre::String				robotNode = "RobotNode";

	const Ogre::String			bronzeSoldierMesh = "BronzeSoldier.mesh";
	const Ogre::String			bronzeArcherMesh = "BronzeArcher.mesh";
	const Ogre::String			bronzeWizardMesh = "BronzeWizard.mesh";
	
	const Ogre::String			skySoldierMesh = "SkySoldier.mesh";
	const Ogre::String			skyArcherMesh = "SkyArcher.mesh";
	const Ogre::String			skyWizardMesh = "SkyWizard.mesh";
};

#endif __Constants_h_
