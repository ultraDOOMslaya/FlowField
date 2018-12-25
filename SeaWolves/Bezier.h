#pragma once

#include "Ogre.h"

class Bezier
{
public:
	Bezier();
	~Bezier();

	static Ogre::Vector3 getPoint(Ogre::Vector3 a, Ogre::Vector3 b, Ogre::Vector3 c, float t);
};

