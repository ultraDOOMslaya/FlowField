#pragma once

#include <Ogre.h>


class MapEditor
{
public:
	MapEditor();
	~MapEditor();

	//TODO this shouldn't be a static class
	static void changeSquareTexture(Ogre::Entity* tile, Ogre::String textureType);
	/*static void changeSquareElevation()*/
};

