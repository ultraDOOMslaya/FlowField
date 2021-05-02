#include "SelectionCircle.h"


SelectionCircle::SelectionCircle(Ogre::SceneManager* mScnMgr, Ogre::Vector3 position, int unitID, int gameObjectType, Ogre::Real circleRadius)
{
	gameSceneManager = mScnMgr;
	//-------------------------------------------------------
	/***  Circle Code ***/
	//-------------------------------------------------------
	Ogre::Real circleStart = 0.0f;
	const int circleResolution = 120;
	int loopCount;

	Ogre::Degree theta = (Ogre::Degree)0;
	Ogre::Degree alpha = (Ogre::Degree)360 / circleResolution;

	Ogre::Vector3 circularCord2[circleResolution];
	Ogre::Vector3 circleCenter;

	circleCenter.x = circleStart;
	circleCenter.y = 0;
	circleCenter.z = 0;

	//Calculating vertices

	for (loopCount = 0; loopCount < circleResolution; loopCount++) {
		theta = theta + alpha;
		circularCord2[loopCount].x = (Ogre::Math::Cos(theta) * circleRadius) + position.x;
		circularCord2[loopCount].y = circleStart + position.y + 1;
		circularCord2[loopCount].z = (Ogre::Math::Sin(theta) * circleRadius) + position.z;
	}

	Ogre::String circleName = "circleXY_" + Ogre::StringConverter::toString(gameObjectType) + "_" + Ogre::StringConverter::toString(unitID);
	circleXY = gameSceneManager->createManualObject(circleName);

	//*** Circle XY ***

	circleXY->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
	circleXY->colour(Ogre::ColourValue(0.0f, 1.0f, 0.0f));

	for (loopCount = 0; loopCount < circleResolution; loopCount++) {
		circleXY->position(circularCord2[loopCount]);
	}
	circleXY->position(circularCord2[0]);
	circleXY->end();

	circleNode = gameSceneManager->getRootSceneNode()->createChildSceneNode();
	circleNode->attachObject(circleXY);
	startPosition = position;
}


SelectionCircle::SelectionCircle(Ogre::SceneManager* mScnMgr, Ogre::Vector3 position, int unitID, int gameObjectType)
{
	gameSceneManager = mScnMgr;
	//-------------------------------------------------------
	/***  Circle Code ***/
	//-------------------------------------------------------
	Ogre::Real circleStart = 0.0f;
	Ogre::Real circleRadius = 18.0f;
	const int circleResolution = 120;
	int loopCount;

	Ogre::Degree theta = (Ogre::Degree)0;
	Ogre::Degree alpha = (Ogre::Degree)360 / circleResolution;

	Ogre::Vector3 circularCord2[circleResolution];
	Ogre::Vector3 circleCenter;

	circleCenter.x = circleStart;
	circleCenter.y = 0;
	circleCenter.z = 0;

	//Calculating vertices

	for (loopCount = 0; loopCount < circleResolution; loopCount++) {
		theta = theta + alpha;
		circularCord2[loopCount].x = (Ogre::Math::Cos(theta) * circleRadius) + position.x;
		circularCord2[loopCount].y = circleStart + position.y + 1;
		circularCord2[loopCount].z = (Ogre::Math::Sin(theta) * circleRadius) + position.z;
	}

	Ogre::String circleName = "circleXY_" + Ogre::StringConverter::toString(gameObjectType) + "_" + Ogre::StringConverter::toString(unitID);
	circleXY = gameSceneManager->createManualObject(circleName);

	//*** Circle XY ***

	circleXY->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
	circleXY->colour(Ogre::ColourValue(0.0f, 1.0f, 0.0f));

	for (loopCount = 0; loopCount < circleResolution; loopCount++) {
		circleXY->position(circularCord2[loopCount]);
	}
	circleXY->position(circularCord2[0]);
	circleXY->end();

	circleNode = gameSceneManager->getRootSceneNode()->createChildSceneNode();
	circleNode->attachObject(circleXY);
	startPosition = position;
}


SelectionCircle::~SelectionCircle()
{
	gameSceneManager->destroyManualObject(circleXY);
}


void SelectionCircle::move(Ogre::Vector3 position) {
	position.y += 1;
	circleNode->setPosition(position - startPosition);		//TODO figure out why Position is off to begin with for some reason
}

