#include "SquareNeighbor.h"



SquareNeighbor::SquareNeighbor(Ogre::Vector2 pos, int dist)
{
	this->position = pos;
	this->distance = dist;
}


SquareNeighbor::SquareNeighbor()
{
}


SquareNeighbor::~SquareNeighbor()
{
}


void SquareNeighbor::setPosition(Ogre::Vector2 val) {
	position = val;
}


Ogre::Vector2 SquareNeighbor::getPosition(void) {
	return position;
}


void SquareNeighbor::setDistance(int val) {
	distance = val;
}


int SquareNeighbor::getDistance(void) {
	return distance;
}
