#include "MapEditor.h"



MapEditor::MapEditor()
{
}


MapEditor::~MapEditor()
{
}


void swapTexture(Ogre::Entity* tile, Ogre::String textureName) {
	tile->setMaterialName(textureName);
}


void MapEditor::changeSquareTexture(Ogre::Entity* tile, Ogre::String textureType) {
	if (textureType == "Grass") {
		swapTexture(tile, "Examples/GrassFloor");
	}
	else if (textureType == "Dirt") {
		swapTexture(tile, "Examples/DirtFloor");
	}
}