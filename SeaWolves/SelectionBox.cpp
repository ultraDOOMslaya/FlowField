#include "SelectionBox.h"



SelectionBox::SelectionBox(const Ogre::String& name)
	: Ogre::ManualObject(name)
{
	setRenderQueueGroup(100);		//Ogre::RENDER_QUEUE_OVERLAY (deprecated I guess)
	
	setUseIdentityProjection(true);		//This flag makes it not work
	setUseIdentityView(true);
	setQueryFlags(0);
	selecting = false;
}


SelectionBox::~SelectionBox()
{
}


void SelectionBox::setCorners(float left, float top, float right, float bottom)
{
	left = 2 * left - 1;
	right = 2 * right - 1;
	top = 1 - 2 * top;
	bottom = 1 - 2 * bottom;

	clear();
	begin("Examples / OgreLogo", Ogre::RenderOperation::OT_LINE_STRIP);
	
	position(left, top, -1);
	position(right, top, -1);
	position(right, bottom, -1);
	position(left, bottom, -1);
	position(left, top, -1);
	
	end();
	setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
}
//----------------------------------------------------------------

void SelectionBox::setCorners(
	const Ogre::Vector2& topLeft,
	const Ogre::Vector2& bottomRight)
{
	setCorners(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}
//----------------------------------------------------------------

/*
* Activate the selection box and set starting bounds.
*/
void SelectionBox::enable(int width, int height) {
	clear();
	int x, y;
	SDL_GetMouseState(&x, &y);
	mStart.x = (float)x / width;
	mStart.y = (float)y / height;
	mStop = mStart;

	selecting = true;
	setVisible(true);
}
//----------------------------------------------------------------

/*
* Disable the selection box.
*/
void SelectionBox::disable() {
	selecting = false;
	setVisible(false);
}
//----------------------------------------------------------------

/*
* On mouse drag event, update the bounds of the select box.
*/
void SelectionBox::checkAndUpdate(int width, int height) {
	if (selecting) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		mStop.x = (float)x / width;
		mStop.y = (float)y / height;

		setCorners(mStart, mStop);
	}
}
//----------------------------------------------------------------