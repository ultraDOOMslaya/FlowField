#pragma once
#ifndef __SquareNeighbor_h_
#define __SquareNeighbor_h_

#include <Ogre.h>


class SquareNeighbor
{
	Ogre::Vector2			position;
	int						distance;

public:
	SquareNeighbor(Ogre::Vector2 pos, int dist);
	SquareNeighbor();
	~SquareNeighbor();

	virtual void setPosition(Ogre::Vector2 val);
	virtual Ogre::Vector2 getPosition(void);

	virtual void setDistance(int val);
	virtual int getDistance(void);
};

#endif __SquareNeighbor_h_