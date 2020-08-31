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

	enum UnitStates {
		Idle = 1,
		Marching = 2,
		Attacking = 3,
		Hunting = 4
	};

	const static int			gridWidth = 15;
	const static int			gridHeight = 15;
	const static int			edgeLength = 60; //was 50
	//const static int			edgeLength = 65; //was 50
	//const static int			dimension = gridWidth;	//TODO: not a good standard... should be fixed because same as gridHeight
	const static int			dimension = 15;	//TODO: not a good standard... should be fixed
	const static int			WALL = std::numeric_limits<int>::max();
	const static int			terrainQueryMask = 1;
	const static int			unitQueryMask = 2;
	const static int			awareRadius = 500;
	const static int			holdPositionFlowValue = 5;

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
