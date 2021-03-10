#pragma once
#ifndef __Constants_h_
#define __Constants_h_

#include <Ogre.h>

/* Clearpoint: the point at which heavy armor units have passed the group and the rest of the group can move (if they don't haveLos). */

class Constants
{
public:
	Constants();
	~Constants();

	/** Ogre Terrain properties **/
	const static int			dimension = 25;	//TODO: not a good standard... should be fixed
	const static int			gridWidth = 25;
	const static int			gridHeight = 25;
	const static int			edgeLength = 80; //was 50
	const static int			worldSize = 2000;

	const static int			physicsShapeHalfLength = (edgeLength / 2) + 1;
	const static int			physicsShapeBuildingHalfLength = 120 + 1;

	/** tile map properties **/
	//const static int			gridWidth = 30;
	//const static int			gridHeight = 30;
	//const static int			edgeLength = 60; //was 50
	//const static int			edgeLength = 65; //was 50
	//const static int			dimension = gridWidth;	//TODO: not a good standard... should be fixed because same as gridHeight
	//TODO the dimension todo isnt kidding this should be looked at.
	
	const static int			WALL = std::numeric_limits<int>::max();
	const static int			terrainQueryMask = 1;
	const static int			unitQueryMask = 2;
	const static int			buildingQueryMask = 3;
	const static int			awareRadius = 500;
	const static int			holdPositionFlowValue = 5;
	const static int			unitBaseHeight = 200;

	const Ogre::String			robot = "robot.mesh";
	Ogre::String				robotNode = "RobotNode";

	const Ogre::String			bronzeSoldierMesh = "BronzeSoldier.mesh";
	const Ogre::String			bronzeArcherMesh = "BronzeArcher.mesh";
	const Ogre::String			bronzeWizardMesh = "BronzeWizard.mesh";
	const Ogre::String			axe = "axe.mesh";
	
	const Ogre::String			skySoldierMesh = "SkySoldier.mesh";
	const Ogre::String			skyArcherMesh = "SkyArcher.mesh";
	const Ogre::String			skyWizardMesh = "SkyWizard.mesh";

	const Ogre::String			heavyArmor = "HeavyArmor";
	const Ogre::String			MediumArmor = "MediumArmor";
	const Ogre::String			LightArmor = "LightArmor";
};

#endif __Constants_h_
