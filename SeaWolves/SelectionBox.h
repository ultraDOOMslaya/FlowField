#pragma once

#include <OgreManualObject.h>

class SelectionBox : public Ogre::ManualObject
{
public:
	SelectionBox(const Ogre::String& name);
	virtual ~SelectionBox();

	void setCorners(float left, float top, float right, float bottom);
	void setCorners(const Ogre::Vector2& topLeft, const Ogre::Vector2& bottomRight);

	Ogre::Vector2				mStart, mStop;
	bool						selecting;
	int							windowWidth, windowHeight;
};

