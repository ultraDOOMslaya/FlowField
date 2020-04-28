#include "GridEditor.h"



GridEditor::GridEditor(Ogre::SceneManager* mScnMgr, std::vector<std::vector<GridSquare*>>* mGridMap)
{
	gameSceneManager = mScnMgr;
	gridMap = mGridMap;
}


GridEditor::~GridEditor()
{
}


void setSquareColor(Ogre::Entity* entity, Ogre::MaterialPtr mat, Ogre::ColourValue cv) {
	entity->setMaterialName(mat->getName());
}
//----------------------------------------------------------------

void GridEditor::addTerrainValue(Ogre::String squareName, Ogre::MaterialPtr	mat, std::vector<GridSquare*>* impassableTerrain, b2World* world) {
	int squareNumber = stoi(squareName);
	int xcount = 0;
	int ycount = 0;
	int total = 0;
	bool end = false;
	for (xcount; xcount < Constants::dimension; xcount++) {
		total = xcount;
		while (total <= squareNumber) {
			if (total == squareNumber) {
				end = true;
				break;
			}
			else {
				total += Constants::dimension;
				ycount++;
			}
		}
		if (end) {
			break;
		}
		else {
			ycount = 0;
		}
	}
	Ogre::Vector2 startpos = GridUtils::numericalCordFinder(Ogre::Vector2(xcount, ycount));

	/* TODO fix unit creation */
	/*if (createUnitMode) {
		Ogre::String robotName = "RobotNode" + Ogre::StringConverter::toString(robotNumber);
		robotNumber++;

		//Unit* unit = new Unit(mScnMgr, Ogre::Vector3(startpos.x, 0.0f, startpos.y), robotName);
		//units.push_back(*unit);
	}*/

	//Change color of square
	Ogre::Entity* entity = gameSceneManager->getEntity(squareName);
	setSquareColor(entity, mat, Ogre::ColourValue::Red);

	//Set the value according to color
	//Ogre::Vector2 terrain = Ogre::Vector2(xcount, ycount);
	GridSquare* gridSquare = new GridSquare(world, xcount, ycount);
	impassableTerrain->push_back(gridSquare);
}

void GridEditor::changeTileColor(int x, int y, Ogre::MaterialPtr mat, Ogre::ColourValue color) {
	//Ogre::MaterialPtr greenMat = gameSceneManager->getEntity("1")->getSubEntity(0)->getMaterial()->clone("BaseGreenNoLighting");
	Ogre::Entity* entity = gameSceneManager->getEntity((*gridMap)[x][y]->entityName);


	setSquareColor(entity, mat, color);
}
