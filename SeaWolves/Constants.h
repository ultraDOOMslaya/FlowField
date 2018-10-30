#pragma once
#ifndef __Constants_h_
#define __Constants_h_

#include <Ogre.h>

class Constants
{
public:
	Constants();
	~Constants();

	const static int			gridWidth = 11;
	const static int			gridHeight = 11;
	const static int			edgeLength = 50;
	const static int			dimension = gridWidth;	//TODO: not a good standard... should be fixed
	const static int			WALL = std::numeric_limits<int>::max();
	const Ogre::String			robot = "robot.mesh";
	Ogre::String				robotNode = "RobotNode";
	
};

#endif __Constants_h_
