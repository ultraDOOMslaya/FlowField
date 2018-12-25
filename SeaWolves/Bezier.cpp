#include "Bezier.h"



Bezier::Bezier()
{
}


Bezier::~Bezier()
{
}


Ogre::Vector3 Bezier::getPoint(Ogre::Vector3 a, Ogre::Vector3 b, Ogre::Vector3 c, float t) {
	float r = 1.0f - t;
	return r * r * a + 2.0f * r * t * b + t * t * c;
}
