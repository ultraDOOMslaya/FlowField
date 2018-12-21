#include "GridEditor.h"



GridEditor::GridEditor(Ogre::SceneManager* mScnMgr)
{
	gameSceneManager = mScnMgr;
}


GridEditor::~GridEditor()
{
}


void setSquareColor(Ogre::Entity* entity, Ogre::MaterialPtr mat, Ogre::ColourValue cv) {
	entity->setMaterialName(mat->getName());
}
//----------------------------------------------------------------

void GridEditor::addTerrainValue(Ogre::String squareName, Ogre::MaterialPtr	mat, std::vector<Ogre::Vector2>* impassableTerrain) {
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
	Ogre::Vector2 terrain = Ogre::Vector2(xcount, ycount);
	impassableTerrain->push_back(terrain);
	
}
