#include "GameRunnable.h"

GameRunnable::GameRunnable()
	: OgreBites::ApplicationContext("SeaWolves"),
	gridMap(Constants::dimension, std::vector<GridSquare*>(Constants::dimension, NULL)),
	mTrayMgr(0),
	mScnMgr(0),
	mCam(0),
	mWindow(0),
	mRayScnQuery(0),
	robotAnimState(NULL),
	mUnitNode(0),
	mWalkList(0),
	mWalkSpeed(35),
	mDirection(0, 0, 0), //direction the object is moving
	mDistance(0), // distance object has left to travel
	mDestination(0, 0, 0), // destination the object is moving towards
	robotEntity(NULL),
	createUnitMode(false),
	currentPos(0, 0),
	robotNumber(0),
	selectBox(0),
	debugx(0),
	debugy(0),
	debugz(0),
	pathLines(0)
{
}
//----------------------------------------------------------------

GameRunnable::~GameRunnable()
{
	
	//if (mTrayMgr) delete mTrayMgr;
	//if (mCam) delete mCam;

	if (mRoot) delete mRoot;
	

	mScnMgr->destroyQuery(volQuery);

	if (gridEditor) delete gridEditor;

	//if (selectBox)
		//delete selectBox;
	//if (gridEditor)
		//delete gridEditor;
}		// Random exception thrown on program close due to GridSquares*
//----------------------------------------------------------------

void GameRunnable::createSquare(int width, int height, int edgeLength, std::string meshName, bool oddOrEven, Ogre::ColourValue color, Ogre::MaterialPtr	defaultMaterial)
{
	Ogre::MeshPtr tileMesh;
	Ogre::ManualObject* manual = NULL;
	{
		manual = mScnMgr->createManualObject(meshName);
		bool doIwantToUpdateItLater = false;
		manual->setDynamic(doIwantToUpdateItLater);
		float triangleScale = 0.7f;
		manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		{
			int cm = width * triangleScale;
			int cp = height * triangleScale;
			edgeLength = edgeLength * triangleScale;
			manual->position(cm, 0, cp);// a vertex
			manual->colour(color);
			manual->position((cm - edgeLength), 0, cp);// a vertex
			manual->colour(color);
			manual->position((cm - edgeLength), 0, (cp + edgeLength));// a vertex
			manual->colour(color);
			manual->position(cm, 0, (cp + edgeLength));// a vertex
			manual->colour(color);
			manual->position(cm, 0, cp);// a vertex
			manual->colour(color);
			manual->position((cm - edgeLength), 0, cp);// a vertex
			manual->colour(color);
			manual->position((cm - edgeLength), 0, (cp + edgeLength));// a vertex
			manual->colour(color);
			manual->position(cm, 0, (cp + edgeLength));// a vertex
			manual->colour(color);
			//face behind / front
			manual->triangle(0, 1, 2);
			manual->triangle(2, 3, 0);
			manual->triangle(4, 6, 5);
			manual->triangle(6, 4, 7);
			//face top / down
			manual->triangle(0, 4, 5);
			manual->triangle(5, 1, 0);
			manual->triangle(2, 6, 7);
			manual->triangle(7, 3, 2);
			//face left / right
			manual->triangle(0, 7, 4);
			manual->triangle(7, 0, 3);
			manual->triangle(1, 5, 6);
			manual->triangle(6, 2, 1);
		}
		manual->end();
		tileMesh = manual->convertToMesh(meshName);
	}
	Ogre::Entity* planeEntity = mScnMgr->createEntity(meshName, tileMesh);
	planeEntity->setQueryFlags(Constants::terrainQueryMask);
	mScnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(planeEntity);
	planeEntity->setCastShadows(false);

	//planeEntity->getSubEntity("0")->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->get
	
	GridSquare* square = new GridSquare(width, height, meshName, color, defaultMaterial);
	int x = (width / Constants::edgeLength) - 1;
	int y = (height / Constants::edgeLength) - 1;
	gridMap[x][y] = square;
}
//----------------------------------------------------------------

void GameRunnable::createTileMap(void)
{
	int xscale, yscale = 0;
	for (int i = 0; i < (Constants::gridHeight * Constants::gridWidth); ++i) {
		int temp = i;
		if ((temp % Constants::gridWidth) == 0) {
			xscale = Constants::edgeLength;
			yscale += Constants::edgeLength;
		}
		else {
			xscale += Constants::edgeLength;
		}
		std::stringstream meshName;
		meshName << i;
		std::string strToPass = meshName.str();

		if (temp % 2 == 0) {
			createSquare(xscale, yscale, Constants::edgeLength, strToPass, true, (Ogre::ColourValue::Blue), mapTileMat1);
		}
		else {
			createSquare(xscale, yscale, Constants::edgeLength, strToPass, false, (Ogre::ColourValue::ColourValue(0.2f, 0.2f, 1.0f, 1.0f)), mapTileMat2);
		}

		if (temp == 0) {
			mat = mScnMgr->getEntity("0")->getSubEntity(0)->getMaterial()->clone("BaseGreenNoLighting");
			Ogre::TextureUnitState* texture = new Ogre::TextureUnitState(mat->getTechnique(0)->getPass(0));
			mat->getTechnique(0)->getPass(0)->addTextureUnitState(texture);
			mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue::Green);

			redMat = mScnMgr->getEntity("0")->getSubEntity(0)->getMaterial()->clone("BaseRedNoLighting");
			Ogre::TextureUnitState* textureRed = new Ogre::TextureUnitState(redMat->getTechnique(0)->getPass(0));
			redMat->getTechnique(0)->getPass(0)->addTextureUnitState(textureRed);
			redMat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue::Red);

			mapTileMat1 = mScnMgr->getEntity("0")->getSubEntity(0)->getMaterial()->clone("BaseBlueNoLighting");
			Ogre::TextureUnitState* textureBlue = new Ogre::TextureUnitState(mapTileMat1->getTechnique(0)->getPass(0));
			mapTileMat1->getTechnique(0)->getPass(0)->addTextureUnitState(textureBlue);
			mapTileMat1->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue::Blue);

			mapTileMat2 = mScnMgr->getEntity("0")->getSubEntity(0)->getMaterial()->clone("BaseLightNoLighting");
			Ogre::TextureUnitState* textureLightBlue = new Ogre::TextureUnitState(mapTileMat2->getTechnique(0)->getPass(0));
			mapTileMat2->getTechnique(0)->getPass(0)->addTextureUnitState(textureLightBlue);
			mapTileMat2->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue::ColourValue(0.2f, 0.2f, 1.0f, 1.0f));
		}
	}
	
}
//----------------------------------------------------------------

void GameRunnable::changeSquareColor(int num) {
	if (num != 0) {
		std::ostringstream s;
		s << "Ogre/MO" << num;

		Ogre::String name(s.str());

		Ogre::Entity* ent = mScnMgr->getEntity(name);
		ent->setMaterialName("Examples/BumpyMetal");
		std::chrono::duration<int, std::milli> timespan(1000);
		std::this_thread::sleep_for(timespan);
	}
}
//----------------------------------------------------------------

bool GameRunnable::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
	}
	if (evt.keysym.sym == OgreBites::SDLK_SPACE)
	{
		createUnitMode = true;
	}
	if (evt.keysym.sym == SDLK_a)
	{
		activePlayer->queuedAttackMove = !activePlayer->queuedAttackMove;
	}
	return true;
}
//----------------------------------------------------------------

bool GameRunnable::mouseMoved(const OgreBites::MouseMotionEvent &evt)
{
	//mTrayMgr->refreshCursor();

	if (selectBox->selecting) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		selectBox->mStop.x = (float)x / getRenderWindow()->getWidth();
		selectBox->mStop.y = (float)y / getRenderWindow()->getHeight();

		selectBox->setCorners(selectBox->mStart, selectBox->mStop);
	}

	return true;
}
//----------------------------------------------------------------

bool GameRunnable::mouseReleased(const OgreBites::MouseButtonEvent &evt)
{
	if (evt.button == SDL_BUTTON_LEFT) {
		// Did we drag or did we click?
		if (selectBox->mStop == selectBox->mStart) {
			Ogre::String objectName = "";
			Ogre::Ray mouseRay = mTrayMgr->getCursorRay(mCam);

			mRayScnQuery->setRay(mouseRay);
			Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
			Ogre::RaySceneQueryResult::iterator it = result.begin();

			if (mModeCB->isChecked()) {
				Ogre::String squareName;
				for (; it != result.end(); it++) {
					if (it->movable->getQueryFlags() == Constants::terrainQueryMask) {
						//gridEditor->addTerrainValue(it->movable->getName(), mat, &impassableTerrain);
						gridEditor->addTerrainValue(it->movable->getName(), redMat, &impassableTerrainSquares, mWorld);
					}
				}
			}
			else if (result.size() > 1) {		//Found the floor object AND something else
				Unit* unit;
				for (; it != result.end(); it++) {
					if (it->movable->getQueryFlags() == Constants::unitQueryMask) {
						std::map<Ogre::String, Unit*>::iterator itTree = units.find(it->movable->getParentSceneNode()->getName());
						unit = itTree->second;
						//TODO make this DRY
						if (activePlayer->unitQueue.size() > 0) {
							PathFinding* path = activePlayer->unitQueue.front()->path;
							for (std::vector<Ogre::Vector2*>::iterator it = path->formationLocations.begin(); it != path->formationLocations.end(); ++it) {
								gridMap[(*it)->x][(*it)->y]->defaultColor(mScnMgr);
							}
							path->formationLocations.clear(); // <-- add this to the path method where the units queue is reset
						}
						activePlayer->focusUnit(unit);
						break;
					}
				}
			}
			else {								//Just the floor object
				//TODO make this DRY
				if (activePlayer->unitQueue.size() > 0) {
					PathFinding* path = activePlayer->unitQueue.front()->path;
					for (std::vector<Ogre::Vector2*>::iterator it = path->formationLocations.begin(); it != path->formationLocations.end(); ++it) {
						gridMap[(*it)->x][(*it)->y]->defaultColor(mScnMgr);
					}
					path->formationLocations.clear(); // <-- add this to the path method where the units queue is reset
				}
				activePlayer->clearUnitQueue();
			}
		}
		else {
			pim->performSelection(selectBox->mStart, selectBox->mStop, activePlayer, gridMap);
			//performSelection(selectBox->mStart, selectBox->mStop);
		}
		selectBox->selecting = false;
		selectBox->setVisible(false);
	}
	else { // Right click on an enemy player?
		Ogre::String objectName = "";
		Ogre::Ray mouseRay = mTrayMgr->getCursorRay(mCam);

		mRayScnQuery->setRay(mouseRay);
		Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
		Ogre::RaySceneQueryResult::iterator it = result.begin();
		
		if (result.size() > 1) {		//Found the floor object AND something else
			Unit* unit;
			for (; it != result.end(); it++) {
				if (it->movable->getQueryFlags() == Constants::unitQueryMask) {
					std::map<Ogre::String, Unit*>::iterator itTree = units.find(it->movable->getParentSceneNode()->getName());
					unit = itTree->second;
					switch (PlayerUtils::determineStatus(activePlayer, players, unit)) 
					{
					case PlayerRelationshipStatus::HOSTILE: activePlayer->attack(unit);
						break;
					default:
						break;
					}
				}
			}
		}
	}
	
	return true;
}
//----------------------------------------------------------------

bool GameRunnable::mousePressed(const OgreBites::MouseButtonEvent &evt)
{
	bool mbRight = false;

	if (evt.button == SDL_BUTTON_RIGHT) {
		mbRight = true;
	}
	
	if (mbRight) {

		Ogre::String objectName = "";
		Ogre::Ray mouseRay = mTrayMgr->getCursorRay(mCam);

		mRayScnQuery->setRay(mouseRay);
		Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
		Ogre::RaySceneQueryResult::iterator it = result.begin();

		for (; it != result.end(); it++)
		{
			objectName = it->movable->getName();
		}

		std::ostringstream oss;
		oss << objectName;
		mCordPanel->setText(oss.str());

		// Used for right click movement... basic click to move
		Ogre::Vector2 SquareIndex = GridUtils::gridIndexFinder(objectName);
		PathFinding* path = new PathFinding(SquareIndex, &impassableTerrainSquares, mScnMgr);
		int shortestClearDistance = 0;
		// Forget this crap just pick the first F'in unit in the queue...
		Ogre::Vector3 conglomerate = SteeringBehaviour::unitGroupConglomerate(&activePlayer->unitQueue);
		//
		path->assignUnitFormationLocations(SquareIndex.x, SquareIndex.y, activePlayer->unitQueue.size(), conglomerate);		//Should probably add this to the constructor of PathFinding
		if (mShowFlowPathCB->isChecked()) {
			path->showFlow(mScnMgr);
		}


		//TODO make this DRY
		if (activePlayer->unitQueue.size() > 0) {
			PathFinding* prevPath = activePlayer->unitQueue.front()->path;
			if (prevPath != NULL) {
				for (std::vector<Ogre::Vector2*>::iterator it = prevPath->formationLocations.begin(); it != prevPath->formationLocations.end(); ++it) {
					gridMap[(*it)->x][(*it)->y]->defaultColor(mScnMgr);
				}
				prevPath->formationLocations.clear();
			}
		}
		gom->proximityLocationFormation(SquareIndex.x, SquareIndex.y, activePlayer->unitQueue, path);

		int finalCordListPos = 0;
		// TODO refactor to allow an active player and unit movement for multiple players
		//for(std::map<Ogre::String, Unit>::iterator it = units.begin(); it != units.end(); ++it) {
		for (std::vector<Unit*>::iterator unit = activePlayer->unitQueue.begin(); unit != activePlayer->unitQueue.end(); ++unit) {

			//TODO temporary for flocking work. Remove or refactor. Would like to see a single method with code the render loop uses.
			Ogre::Vector2 aPos = GridUtils::numericalCordFinder(path->flowField[(*unit)->currentPos.x][(*unit)->currentPos.y]);
			b2Vec2* nextCord = new b2Vec2(aPos.x, aPos.y);

			(*unit)->b2WalkList.push_back(*nextCord);

			/** Experimental Code **/
			Ogre::Vector2 finalCord = *(path->formationLocations[finalCordListPos]);
			(*unit)->debugPos1 = finalCord;
			
			/** End Experimental Code **/

			//b2Vec2 finalPosition = GridUtils::b2NumericalCordFinder(SquareIndex);
			//Ogre::Vector3* finalCord = new Ogre::Vector3(finalPosition.x, 0, finalPosition.y);

			//Working code before more sucienct formation code
			//b2Vec2 finalPosition = GridUtils::b2NumericalCordFinder(finalCord);
			//(*unit)->b2FinalDestination = finalPosition;
			//(*unit)->finalDestination = Ogre::Vector3(finalPosition.x, 0, finalPosition.y);

			/* Reset all unit behaviours */
			(*unit)->halt();
			(*unit)->attacking = false;
			(*unit)->hunting = false;

			if (activePlayer->queuedAttackMove) {
				activePlayer->attackMove();
			}

			//TODO encapsulate this
			if ((*unit)->path != NULL) {
				if ((*unit)->path->pathingUnits <= 1) {
					delete (*unit)->path;
				}
				else {
					(*unit)->path->pathingUnits--;
				}
			}

			int distanceToDestination = (*unit)->getPosition().squaredDistance(GridUtils::numericalCordFinder(finalCord.x, finalCord.y));

			if (path->clearPointDistance == 0) {
				path->clearPointDistance = distanceToDestination;
			}
			else if (path->clearPointDistance > distanceToDestination){
				path->clearPointDistance = distanceToDestination;
			}

			(*unit)->path = path;
			(*unit)->path->pathingUnits++;
				
			++finalCordListPos;
		}

		
		int placed = 0;
		int armoredUnitCount = 0;
		Constants constants;
		//Find amount of armored units
		/*for (std::vector<Unit*>::iterator unit = activePlayer->unitQueue.begin(); unit != activePlayer->unitQueue.end(); ++unit) {
			if ((*unit)->mUnitClass == constants.heavyArmor) {
				armoredUnitCount++;
				std::rotate(activePlayer->unitQueue.begin(), unit, unit + 1);
			}
		}*/

		/* --- Moving final destination code to the GameObjectManager ---
		//Place units in formation due to their pecking order and closest distance to position being evaluated.
		for (std::vector<Unit*>::iterator unit = activePlayer->unitQueue.begin(); unit != activePlayer->unitQueue.end(); ++unit) {
			std::multimap<int, Ogre::Vector2>::iterator position = path->mappedFormation.begin();
			int rowColumnKey = position->first;
			float shortestDistance = 0.0;
			Ogre::Vector2 shortestDestinationCord = position->second;;
			while (position->first == rowColumnKey) {
				if (shortestDistance = 0) {
					shortestDistance = (*unit)->getPosition().squaredDistance(GridUtils::numericalCordFinder(Ogre::Vector3(position->second.x, 0, position->second.y)));
					shortestDestinationCord = position->second;
				}
				else if (shortestDistance > (*unit)->getPosition().squaredDistance(GridUtils::numericalCordFinder(Ogre::Vector3(position->second.x, 0, position->second.y)))) {
					shortestDistance = (*unit)->getPosition().squaredDistance(GridUtils::numericalCordFinder(Ogre::Vector3(position->second.x, 0, position->second.y)));
					shortestDestinationCord = position->second;
				}
				
				position++;
			}
			path->mappedFormation.erase(path->mappedFormation.begin());
			(*unit)->debugPos1 = shortestDestinationCord;
			b2Vec2 finalPosition = GridUtils::b2NumericalCordFinder(shortestDestinationCord);
			/** End Experimental Code **/
		/*
			//b2Vec2 finalPosition = GridUtils::b2NumericalCordFinder(SquareIndex);
			//Ogre::Vector3* finalCord = new Ogre::Vector3(finalPosition.x, 0, finalPosition.y);
			(*unit)->b2FinalDestination = finalPosition;
			(*unit)->finalDestination = Ogre::Vector3(finalPosition.x, 0, finalPosition.y);
		}
		*/

		/* Reset state player state */
		activePlayer->queuedAttackMove = false;
		
		if (activePlayer->unitQueue.size() > 0) {
			for (std::vector<Ogre::Vector2*>::iterator it = path->formationLocations.begin(); it != path->formationLocations.end(); ++it) {
				Ogre::Vector2* point = *it;
				mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue::Green);

				gridEditor->changeTileColor(point->x, point->y, mat, Ogre::ColourValue::Green);

			}
		}
		
		//mWalkList.push_back(Ogre::Vector3(cords.x, 0, cords.y));
	}
	else {
		selectBox->clear();
		int x, y;
		SDL_GetMouseState(&x, &y);
		selectBox->mStart.x = (float)x / getRenderWindow()->getWidth();
		selectBox->mStart.y = (float)y / getRenderWindow()->getHeight();
		selectBox->mStop = selectBox->mStart;

		selectBox->selecting = true;
		selectBox->setVisible(true);
		
		/*
		Ogre::Vector3 clickableSquareCords = Ogre::Vector3(startpos.x, 0.0f, startpos.y);

		std::ostringstream oss;
		oss << " for " << xcount << "," << ycount;
		mCordPanel->setText(oss.str());
		*/
	}
	//}
	
	return true;
}
//----------------------------------------------------------------

/*void AssignGridFormation(PlayerManager* activePlayer) {
	for (std::vector<Unit*>::iterator unit = activePlayer->unitQueue.begin(); unit != activePlayer->unitQueue.end(); ++unit) {
		if ((*unit)->mUnitClass == "HeavyArmor") {
			int furthestDistance = (*unit)->path->formationLocations
		}
	}
}*/
//----------------------------------------------------------------

void GameRunnable::setup(void)
{
	// do not forget to call the base first
	OgreBites::ApplicationContext::setup();
	addInputListener(this);
	SDL_ShowCursor(SDL_DISABLE);
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	Ogre::Root* root = getRoot();
	Ogre::ConfigDialog* dialog = OgreBites::getNativeConfigDialog();


	OgreBites::TrayListener* mTrayListener = new OgreBites::TrayListener();
	mScnMgr = root->createSceneManager();
	mScnMgr->addRenderQueueListener(getOverlaySystem());
	mTrayMgr = new OgreBites::TrayManager("InterfaceName", getRenderWindow(), mTrayListener);
	mTrayMgr->showFrameStats(OgreBites::TrayLocation::TL_BOTTOMLEFT);
	mCordPanel = mTrayMgr->createTextBox(OgreBites::TL_BOTTOMRIGHT, "MouseCords", "Mouse Cordinates", 175.0f, 125.0f);
	//mModeWidget = mTrayMgr->createParamsPanel(OgreBites::TL_TOPRIGHT, "ModeSet",  Ogre::Real(175), 3);
	mModeCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "WallGen", "Editor Mode", 150.0f);
	mUnitCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "UnitGen", "New Units", 150.0f);
	mShowFlowPathCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "ShowPath", "Show Paths", 150.0f);
	addInputListener(mTrayMgr);

	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(mScnMgr);

	camNode = mScnMgr->getRootSceneNode()->createChildSceneNode("camNode");
	mCam = mScnMgr->createCamera("myCam");
	camNode->setPosition(500, 750, 1000);
	camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TransformSpace::TS_WORLD);
	mCam->setNearClipDistance(5);
	camNode->attachObject(mCam);
	Ogre::Viewport* vp = getRenderWindow()->addViewport(mCam);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	mCam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	root->addFrameListener(this);
	
	player1 = PlayerManager();
	player2 = PlayerManager();
	PlayerRelationship* relationship = new PlayerRelationship(1, PlayerRelationshipStatus::HOSTILE);
	player1.relationship = relationship;
	player2.relationship = relationship;

	activePlayer = &player1;
	players.push_back(&player1);
	players.push_back(&player2);

	//gridMap = new std::vector<std::vector<GridSquare*>>();
	createTileMap();

	mScnMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	//scnMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
	Ogre::Light* spotLight = mScnMgr->createLight("SpotLight");
	spotLight->setDiffuseColour(0, 0, 1.0);
	spotLight->setSpecularColour(0, 0, 1.0);
	spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
	Ogre::SceneNode* spotLightNode = mScnMgr->getRootSceneNode()->createChildSceneNode();
	spotLightNode->attachObject(spotLight);
	spotLightNode->setDirection(-1, -1, 0);
	spotLightNode->setPosition(Ogre::Vector3(200, 200, 0));
	spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));

	mRayScnQuery = mScnMgr->createRayQuery(Ogre::Ray());
	mRayScnQuery->setSortByDistance(true);

	/* TESTING ZONE */

	selectBox = new SelectionBox("SelectionBox");
	mScnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(selectBox);

	volQuery = mScnMgr->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList());
	gridEditor = new GridEditor(mScnMgr, &gridMap);

	//Box2D
	//b2Vec2 gravity(0.0f, -10.0f);
	b2Vec2 gravity(0.0f, 0.0f);
	mWorld = new b2World(gravity);
	//GenerateUnits::generateFourBronze(mScnMgr, &units, &player1.myArmy, mWorld);
	//GenerateUnits::generateOneBronze(mScnMgr, &units, &player1.myArmy, mWorld);
	//GenerateUnits::generateFourBronze(mScnMgr, &units, &player1.myArmy);
	GenerateUnits::generateEightBronze(mScnMgr, &units, &player1.myArmy, mWorld);

	//GenerateUnits::generateOneSky(mScnMgr, &units, &player2.myArmy);
	//GenerateUnits::generateFourSky(mScnMgr, &units, &player2.myArmy);

	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 10;
	int32 positionIterations = 8;

	/* END TESTING ZONE */

	/** Initialize Singletons **/
	pim = new PlayerInputManager(mCam, volQuery, mScnMgr);
	gom = new GameObjectManager();
}
//----------------------------------------------------------------

static Ogre::Vector3* subtractVector(Ogre::Vector3 vec1, Ogre::Vector3 vec2) {
	int x, y, z;
	x = vec1.x - vec2.x;
	y = vec1.y - vec2.y;
	z = vec1.z - vec2.z;
	Ogre::Vector3* vec3 = new Ogre::Vector3(x, y, z);
	return vec3;
}

//TODO remove this duplicate copy in steeringbehaviour
static int distanceTo(Ogre::Vector3 unit1, Ogre::Vector3 unit2) {
	int a = unit1.x - unit2.x;
	int b = unit1.z - unit2.z;

	int c = std::sqrt(a*a + b*b);
	return c;
}
//----------------------------------------------------------------

void GameRunnable::frameRendered(const Ogre::FrameEvent& evt)
{
	/* Camera movement */
	// Working code but annoying when trying to debug
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
	int mMoveScale = 250;
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (x > (getRenderWindow()->getWidth() - 20)) {
	transVector.x += mMoveScale;
	}
	else if (x < 20) {
	transVector.x -= mMoveScale;
	}
	else if (y >(getRenderWindow()->getHeight() - 20)) {
	transVector.y -= mMoveScale;
	}
	else if (y < 20) {
	transVector.y += mMoveScale;
	}
	mScnMgr->getSceneNode("camNode")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 10;
	int32 positionIterations = 8;
	mWorld->Step(timeStep, velocityIterations, positionIterations);
	mWorld->ClearForces();
	for (std::map<Ogre::String, Unit*>::iterator it = units.begin(); it != units.end(); ++it) {
		Unit* unit = it->second;

		/* Attack Code
		if (unit->isAggressive()) {
			if (unit->hasTarget()) {
				unit->distanceFromTarget = unit->getPosition().squaredLength() - unit->mTarget->getPosition().squaredLength();
				if (unit->inRange()) {
					unit->attack();
				}
				else {
					if (unit->isHunting()) {
						CombatBehaviour::huntForTarget(&units, activePlayer, players, unit);
					}
					if (unit->unitAnimState->getAnimationName() != "Walk") {
						unit->walkList.push_back(unit->mTarget->getPosition());
					}
					else {
						unit->finalDestination = unit->mTarget->getPosition();
					}
				}
			}
			else {
				CombatBehaviour::seekTarget(&units, activePlayer, players, unit);
			}
		}*/

		if (unit->unitAnimState->getAnimationName() == "Walk") {
			
			/* Realtime directing */
		
			Ogre::Vector2 b4Position = GridUtils::cordNumericalFinder(unit->getPosition());
			unit->debugB2Pos1 = unit->getB2DPosition();
			//Ogre::Vector2 position = GridUtils::cordNumericalFinder(unit->getPosition()); // Temporary to see if this works over the next line.
			Ogre::Vector2 position = GridUtils::b2CordNumericalFinder(unit->getB2DPosition()); //<------ On a second click y value ends up being -6000 or something
			if (position != unit->currentPos) {
				unit->currentPos = position;
			}

			b2Vec2 cords;
			//Ogre::Vector3 cords = Ogre::Vector3::ZERO;
			if (unit->hasLos()) {
				cords = unit->b2FinalDestination;
			}
			else {
				Ogre::Vector2 direction = GridUtils::numericalCordFinder(*unit->getCurrentFlowValue());
				unit->debugPos1 = *unit->getCurrentFlowValue();
				cords = GridUtils::b2NumericalCordFinder(*unit->getCurrentFlowValue());
				/*int x, y;
				x = direction.x;
				y = direction.y;
				cords = Ogre::Vector3(x, 0, y);*/
			}
			
			/* Physics applied to everyone */
			unit->debugPos2 = Ogre::Vector2(cords.x, cords.y);

			b2Vec2 seek = SteeringBehaviour::seek(unit, cords);
			//b2Vec2 seperation = SteeringBehaviour::b2seperation(unit, &units, activePlayer, players);
			//seperation.operator*=(3);
			//Ogre::Vector3 seperation = SteeringBehaviour::seperation(unit, &units, activePlayer, players);
			

			/* Physics applied to groups */
			//Ogre::Vector3 cohesion = SteeringBehaviour::cohesion(unit, unit->group);
			//Ogre::Vector3 alignment = SteeringBehaviour::alignment(unit, unit->group);
			//Ogre::Vector3 collision = SteeringBehaviour::staticObjectCollisionForceApplier(unit);
			//collision.operator*=(2);

			//unit->forceToApply = seek.operator*=(0.75).operator+=(seperation.operator*=(15)).operator+=(cohesion.operator*=(0.5)).operator+=(alignment.operator/=(3)).operator+=(collision.operator*=(3));
			//unit->forceToApply = seek.operator+=(seperation.operator*=(10)).operator+=(cohesion.operator*=(0.1)).operator+=(alignment).operator+=(collision);
			//unit->forceToApply = seek.operator+=(seperation.operator*=(10)).operator+=(cohesion.operator*=(0.1)).operator+=(alignment);
			//unit->forceToApply = seek.operator*=(6).operator+=(seperation.operator*=(30)).operator+=(cohesion.operator*=(0.1)).operator+=(alignment.operator*=(2)).operator+=(collision.operator*=(2));
			unit->b2ForceToApply = seek;
			
			//Ogre::Vector3 collision = staticObjectCollisionForceApplier(unit);
			//SteeringBehaviour::halt(unit, unit->group); //expr

			//TODO put this somewhere... do it as soon as you see this!
			if (unit->hasArrived()) {
				unit->halt();
			}
		}
	}

	Ogre::AnimationState* tempState;
	for (std::map<Ogre::String, Unit*>::iterator it = units.begin(); it != units.end(); ++it) {
		Unit* unit = it->second;

		if (unit->nextLocation()) {
			unit->animate("Walk");
			//unit->velocity = unit->direction;
			unit->b2Velocity = unit->b2Direction;
			unit->mBody->SetType(b2_dynamicBody);
		}


		if (unit->b2Destination != b2Vec2_zero) {

			//if ((unit->mUnitClass != "HeavyArmor" && !unit->clearToMove()) || unit->groupHasLos()) {
			//if (unit->mUnitClass != "HeavyArmor" && !unit->clearToMove()) {
			if (unit->groupHasLos()) {
				unit->b2Velocity(0.0f);
				unit->mBody->SetLinearVelocity(b2Vec2_zero);
			}
			else {
				//unit->velocity.operator+=(unit->forceToApply.operator*=(evt.timeSinceLastFrame));		//Delta Time
				//unit->b2ForceToApply.operator*=(evt.timeSinceLastFrame);								//Real Time
				unit->b2ForceToApply.operator*=(0.0166666);
				unit->b2Velocity.operator+=(unit->b2ForceToApply);
				//unit->velocity.operator+=(unit->forceToApply.operator*=(0.0166666));					//Mocked Delta Time
				//unit->velocity.operator+=(unit->forceToApply.operator*=(0.00166666));					//Mocked Delta Time
				float speed = unit->b2Velocity.Length();
				if (speed > unit->maxSpeed) {
					unit->b2Velocity.operator*=(unit->maxSpeed / speed);
				}
			}// clearPoint logic

			unit->rotate(unit->b2Velocity);

			//Ogre::Vector3 newPos = unit->getPosition().operator+=(unit->velocity.operator*(0.0166666));
			//Ogre::Vector3 newPos = unit->getPosition().operator+=(unit->velocity.operator*(0.00166666));			
			Ogre::Vector3 newPos = unit->getPosition().operator+=(unit->velocity.operator*(0.00166666));			//Mocked Delta Time
			//Ogre::Vector3 newPos = unit->getPosition().operator+=(unit->velocity.operator*(evt.timeSinceLastFrame));		//Real Time
			//unit->commandMove(newPos);

			unit->mBody->SetLinearVelocity(unit->b2Velocity);
			Ogre::Vector3 moveGraphic = Ogre::Vector3(unit->getB2DPosition().x, 0, unit->getB2DPosition().y);
			unit->commandMove(moveGraphic);

			//unit->mBody->SetLinearVelocity(b2Vec2(newPos.x, newPos.z));
			//unit->mBody->SetAngularVelocity(unit->velocity.length());
		}
		else {
			unit->b2Velocity(0.0f);
			unit->mBody->SetLinearVelocity(b2Vec2_zero);
			unit->mBody->SetType(b2_staticBody);
		}

		// I'm not sure what this code is doing... Commenting it out doesn't seem to break anything
		//Ogre::Vector3 moveGraphic = Ogre::Vector3(unit->getB2DPosition().x, 0, unit->getB2DPosition().y);
		//unit->commandMove(moveGraphic);
		
		/** Advance Unit Animations **/
		unit->unitAnimState->addTime(evt.timeSinceLastEvent);
	}
	
	/*
	Ogre::AnimationState* tempState;
	for (std::map<Ogre::String, Unit*>::iterator it = units.begin(); it != units.end(); ++it) {
		Unit* unit = it->second;
	
		if (unit->nextLocation()) {
			unit->animate("Walk");
			unit->velocity = unit->direction;
		}
		if (unit->destination != Ogre::Vector3::ZERO) {
			Ogre::Real x, z;
			x = unit->mBody->GetPosition().x;
			z = unit->mBody->GetPosition().y;
			Ogre::Vector3 correctPos = Ogre::Vector3(x, 0, z);
			unit->unitNode->setPosition(correctPos);
			if (unit->isSelected) {
				unit->selectionCircle->move(correctPos);
			}

			unit->velocity.operator+=(unit->forceToApply.operator*=(evt.timeSinceLastFrame));		//Delta Time
			//unit->velocity.operator+=(unit->forceToApply.operator*=(0.0166666));					//Mocked Delta Time
			//unit->velocity.operator+=(unit->forceToApply.operator*=(0.00166666));					//Mocked Delta Time
			float speed = unit->velocity.length();
			if (speed > unit->maxSpeed) {
				unit->velocity.operator*=(unit->maxSpeed / speed);
			}

			unit->rotate(unit->velocity);

			
			//Ogre::Vector3 newPos = unit->getPosition().operator+=(unit->velocity.operator*(0.0166666));
			//Ogre::Vector3 newPos = unit->getPosition().operator+=(unit->velocity.operator*(0.00166666));
			Ogre::Vector3 newPos = unit->getPosition().operator+=(unit->velocity.operator*(evt.timeSinceLastFrame));
			unit->commandMove(newPos);
		}
		unit->unitAnimState->addTime(evt.timeSinceLastEvent);		
	}
	*/
	mTrayMgr->frameRendered(evt);
}
//----------------------------------------------------------------

 void GameRunnable::getAllNeighbors(Unit unit) {
	int num = 0;
	int x, y;
	x = unit.currentPos.x;
	y = unit.currentPos.y;
	bool topEdge = false;
	bool bottomEdge = false;
	bool leftEdge = false;
	bool rightEdge = false;
	std::queue<Ogre::Vector2> neighbors;
	if ((x - 1) < 0) {
		leftEdge = true;
	}
	else if ((x + 1) > Constants::dimension) {
		rightEdge = true;
	}

	if ((y - 1) < 0) {
		topEdge = true;
	}
	else if ((y + 1) > Constants::dimension) {
		bottomEdge = true;
	}
	if (topEdge && leftEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (topEdge && rightEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
	}
	else if (bottomEdge && leftEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (bottomEdge && rightEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
	}
	else if (topEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (bottomEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (leftEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
		neighbors.push(Ogre::Vector2(x, y + 1));
	}
	else if (rightEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x, y + 1));
	}
	else {
		//all neighbors are valid
		//neighbors = new Ogre::Vector2[9];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}

	while (!neighbors.empty()) {
		Ogre::Vector2 neighbor = neighbors.front();
		int x, y;
		x = neighbor.x;
		y = neighbor.y;
		Ogre::Vector2 neighborPoint = Ogre::Vector2(x, y);
		int distance = unit.path->dijkastraGrid[x][y];
		SquareNeighbor* unitNeighbor = new SquareNeighbor(neighborPoint, distance);

		unitNeighbors.push(unitNeighbor);
		neighbors.pop();
	}
}


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		GameRunnable app;
		try {
			app.initApp();
			app.getRoot()->startRendering();
			app.closeApp();
			return 0;
		}
		catch (Ogre::Exception& e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occurred: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}
		return 0;
	}
#ifdef __cplusplus
}
#endif