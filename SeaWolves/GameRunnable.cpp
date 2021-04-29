#include "GameRunnable.h"

//TODO move this not in this class
namespace Offsets {
	/** Map builders **/
	extern const float worldSize = 2000;
	extern const float baseHeight = 200;

	extern const int cellSize = 80;
	extern const float heightOffset = 200;

	extern const float brushSize = 0.025;
}

namespace TerrainType {
	extern const Ogre::String Grass = "grass";
	extern const Ogre::String Rock = "rock";
	extern const Ogre::String Sand = "sand";
}

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

void GameRunnable::createTileMap(void)
{
	//TODO there is a lot of redundancy and legacy here
	// a lot of the if logic here can be phased out
	// createTile has redundant variables and I don't see a need to fix it against a triangle scale anymore

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

		//// Checkered grid
		/*if (temp % 2 == 0) {
			createSquare(xscale, yscale, Constants::edgeLength, strToPass, true, (Ogre::ColourValue::Blue), mapTileMat1);
		}
		else {
			createSquare(xscale, yscale, Constants::edgeLength, strToPass, false, (Ogre::ColourValue::ColourValue(0.2f, 0.2f, 1.0f, 1.0f)), mapTileMat2);
		}*/
		//createSquare(xscale, yscale, Constants::edgeLength, strToPass, false, (Ogre::ColourValue::ColourValue(0.2f, 0.2f, 1.0f, 1.0f)), mat);
		//createPlane(xscale, yscale, Constants::edgeLength, strToPass, false, (Ogre::ColourValue::ColourValue(0.2f, 0.2f, 1.0f, 1.0f)), mat);

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

	//TODO eventually follow this pattern:
	// 1) get the status of everything selected by the player
	// 2) execute the input against every selected item
	if (activePlayer->buildingQueue.size() > 0) {
		if (evt.keysym.sym == SDLK_q)
		{
			Ogre::Vector2 spawnPoint = activePlayer->buildingQueue.at(0)->getSpawnPoint();
			activePlayer->mLastUnitId = activePlayer->mLastUnitId++;
			GenerateUnits::generatUnit(mScnMgr, &units, &player1.myArmy, mWorld, &impassableTerrain, mUnitController, activePlayer->mLastUnitId, spawnPoint);
		}
	}

	return true;
}
//----------------------------------------------------------------

bool GameRunnable::mouseMoved(const OgreBites::MouseMotionEvent &evt)
{
	selectBox->checkAndUpdate(getRenderWindow()->getWidth(), getRenderWindow()->getHeight());

	return true;
}
//----------------------------------------------------------------

Ogre::Vector2 getCell(Ogre::Vector3 centerPosition)
{
	int x = centerPosition.x;
	int z = centerPosition.z;

	/*int cell_x = x % Offsets::cellSize > 0 ? (x / Offsets::cellSize) + 1 : x / Offsets::cellSize;
	int cell_z = z % Offsets::cellSize > 0 ? (z / Offsets::cellSize) + 1 : z / Offsets::cellSize;*/

	int cell_x = x / Offsets::cellSize;
	int cell_z = z / Offsets::cellSize;

	return Ogre::Vector2(cell_x, cell_z);
}
//----------------------------------------------------------------

bool GameRunnable::mouseReleased(const OgreBites::MouseButtonEvent &evt)
{
	OgreBites::Event fullEvt = OgreBites::Event();
	fullEvt.button = evt;
	selectBox->disable();

	Ogre::String objectName = "";
	Ogre::Ray mouseRay = mTrayMgr->getCursorRay(mCam);

	Ogre::Ray terrainMouseRay = mTrayMgr->getCursorRay(mCam);
	Ogre::TerrainGroup::RayResult rayResult = mMapManager->getRayResult(&mouseRay);

	mRayScnQuery->setRay(mouseRay);
	Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
	Ogre::RaySceneQueryResult::iterator it = result.begin();
	int rayQuerySize = result.size();

	bool isImpassable = false;
	Ogre::Vector2 gridCords = GridUtils::cordNumericalFinder(rayResult.position);
	for (auto terrain = impassableTerrain.begin(); terrain != impassableTerrain.end(); ++terrain)
	{
		if ((*terrain)->x == gridCords.x && (*terrain)->y == gridCords.y)
			isImpassable = true;
	}

	std::ostringstream oss;
	oss << "wp_x: " << rayResult.position.x << " wp_z: " << rayResult.position.z << "\n";
	oss << "gridX: " << gridCords.x << " gridY: " << gridCords.y << "\n";
	oss << "is impassable: " << isImpassable;
	mCordPanel->setText(oss.str());

	if (evt.button == SDL_BUTTON_LEFT) {
		// Did we drag or did we click?
		if (selectBox->mStop == selectBox->mStart) {
			if (rayQuerySize >= 1) {
				//Found the floor object AND something else
				Unit* unit; //TODO eventually we will have buildings and resources. Make this a gameobject and have unit inherit from it.
				Building* building;
				for (; it != result.end(); it++) {
					if (it->movable->getQueryFlags() == Constants::buildingQueryMask) {
						activePlayer->focusBuilding(mRedBuilding);
						break;
					}
					if (it->movable->getQueryFlags() == Constants::unitQueryMask) {
						std::map<Ogre::String, Unit*>::iterator itTree = units.find(it->movable->getParentSceneNode()->getName());
						unit = itTree->second;

						pim->clearFocusedLocations(activePlayer, gridMap);
						activePlayer->focusUnit(unit);
						break;
					}
				}
			}
			else {
				//Just the floor object
				pim->clearFocusedLocations(activePlayer, gridMap);
				activePlayer->clearUnitQueue();
				activePlayer->clearBuildingQueue();
			}
		}
		else {
			pim->performSelection(selectBox->mStart, selectBox->mStop, activePlayer, gridMap);
		}
		selectBox->disable();
	}

	if (evt.button == SDL_BUTTON_RIGHT) {

		if (!rayResult.hit && result.size() == 0)
			return true;

		for (; it != result.end(); it++)
		{
			objectName = it->movable->getName();
		}
		// Used for right click movement... basic click to move
		Ogre::Vector2 SquareIndex = getCell(rayResult.position);
		//TODO why is squareindex capitalized?
		//Ogre::Vector2 SquareIndex = GridUtils::gridIndexFinder(objectName);

		PathFinding* path = new PathFinding(SquareIndex, &impassableTerrain, mScnMgr, activePlayer->unitQueue.size(), activePlayer->unitGroupConglomerate());

		//TODO move this to a UI manager that path will then check on creation
		if (mShowFlowPathCB->isChecked()) {
			path->showFlow(mScnMgr);
		}

		pim->clearFocusedLocations(activePlayer, gridMap);

		for (std::vector<Unit*>::iterator unit = activePlayer->unitQueue.begin(); unit != activePlayer->unitQueue.end(); ++unit) {
			/* Reset all unit behaviours */
			(*unit)->halt();
			(*unit)->attacking = false;
			(*unit)->trekking = true;
			//(*unit)->hunting = false;

			//TODO temporary for flocking work. Remove or refactor. Would like to see a single method with code the render loop uses.
			Ogre::Vector2 aPos = GridUtils::numericalCordFinder(path->flowField[(*unit)->currentPos.x][(*unit)->currentPos.y]);
			b2Vec2* nextCord = new b2Vec2(aPos.x, aPos.y);
			(*unit)->b2WalkList.push_back(*nextCord);

			b2Vec2 finalPosition = GridUtils::b2NumericalCordFinder(SquareIndex);
			Ogre::Vector3* finalCord = new Ogre::Vector3(finalPosition.x, 0, finalPosition.y);
			(*unit)->b2FinalDestination = finalPosition;
			(*unit)->postCombatB2Desination = finalPosition;
			(*unit)->finalDestination = Ogre::Vector3(finalPosition.x, 0, finalPosition.y);

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

			(*unit)->path = path;
			(*unit)->path->pathingUnits++;
			(*unit)->handleInput(fullEvt);
		}

		//TODO this may be the wrong place for this after the refactor
		/* Reset state player state */
		//activePlayer->queuedAttackMove = false;

		if (mShowFlowPathCB->isChecked()) {
			if (activePlayer->unitQueue.size() > 0) {
				//Ogre::Texture grassTexture = Ogre::Texture
				for (std::vector<Ogre::Vector2*>::iterator it = path->formationLocations.begin(); it != path->formationLocations.end(); ++it) {
					Ogre::Vector2* point = *it;

					//mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue::Green);
					
					//gridEditor->changeTileColor(point->x, point->y, mat, Ogre::ColourValue::Green);

				}
			}
		}

		if (!mNonCombat->isChecked()) {
			Ogre::RaySceneQueryResult::iterator it = result.begin();
			// Right click on an enemy player?
			if (rayQuerySize > 0) {
				//Found the floor object AND something else
				for (; it != result.end(); it++) {
					if (it->movable->getQueryFlags() == Constants::unitQueryMask) {
						std::map<Ogre::String, Unit*>::iterator itTree = units.find(it->movable->getParentSceneNode()->getName());
						Unit& unit = *itTree->second;
						switch (PlayerUtils::determineStatus(activePlayer, players, &unit))
						{
						case PlayerRelationshipStatus::HOSTILE: activePlayer->attack(&unit);
							break;
						default:
							break;
						}
					}

					if (it->movable->getQueryFlags() == Constants::naturalResourceMask) {
						std::map<Ogre::String, NaturalResource*>::iterator itTree = natResources.find(it->movable->getParentSceneNode()->getName());
						NaturalResource& natResource = *itTree->second;
						activePlayer->harvest(&natResource);
					}
				}
			}
		}
		
	}

	if (evt.button == SDL_BUTTON_LEFT) {
		//Editor stuff
		
		Ogre::String squareName;
		for (; it != result.end(); it++) {
			if (it->movable->getQueryFlags() == Constants::terrainQueryMask) {
				if (mModeCB->isChecked()) {
					gridEditor->addTerrainValue(it->movable->getName(), redMat, mWorld, &impassableTerrain);
				}
				//if (mAlterTextureCB->isChecked()) {
				//	MapEditor::changeSquareTexture(mScnMgr->getEntity(it->movable->getName()), mGroundTypeSM->getSelectedItem());
				//}
				//if (mAlterElevationCB->isChecked()) {
				//	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
				//	Ogre::Real elevationStep = 60;
				//	Ogre::Real elevation = mElevationSlider->getValue();
				//	Ogre::Entity* obj = mScnMgr->getEntity(it->movable->getName());
				//	Ogre::SceneNode* node = obj->getParentSceneNode();
				//	transVector.y += (elevation * elevationStep);
				//	node->translate(transVector, Ogre::Node::TS_LOCAL);
				//	/*mScnMgr->getSceneNode("camAnchor")->translate(transVector* evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);*/

				//}
			}
		}
	}
	
	return true;
}
//----------------------------------------------------------------

bool GameRunnable::mousePressed(const OgreBites::MouseButtonEvent &evt)
{
	selectBox->enable(getRenderWindow()->getWidth(), getRenderWindow()->getHeight());	
	
	return true;
}
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
	mCordPanel = mTrayMgr->createTextBox(OgreBites::TL_BOTTOMRIGHT, "MouseCords", "Mouse Cordinates", 225.0f, 175.0f);
	//mModeWidget = mTrayMgr->createParamsPanel(OgreBites::TL_TOPRIGHT, "ModeSet",  Ogre::Real(175), 3);
	mModeCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "WallGen", "Editor Mode", 150.0f);
	mUnitCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "UnitGen", "New Units", 150.0f);
	mShowFlowPathCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "ShowPath", "Show Paths", 150.0f);
	mNonCombat = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "NonCombat", "Non Combat", 150.0f);
	mMoveableCamera = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "MoveableCamera", "Moveable Camera", 150.0f);

	mAlterTextureCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPLEFT, "EditTexture", "Edit Texture", 180.0f);
	mGroundTypeSM = mTrayMgr->createThickSelectMenu(OgreBites::TL_TOPLEFT, "GroundType", "Textures:", 180.0f, 2, { "Grass", "Dirt" });
	mAlterElevationCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPLEFT, "EditElevation", "Edit Elevation", 180.0f);
	mElevationSlider = mTrayMgr->createThickSlider(OgreBites::TL_TOPLEFT, "Elevation", "#:", 180.0f, 150.0f, 0, 6, 7);

	addInputListener(mTrayMgr);

	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(mScnMgr);

	camAnchor = mScnMgr->getRootSceneNode()->createChildSceneNode("camAnchor");
	camNode = camAnchor->createChildSceneNode("camNode");
	mCam = mScnMgr->createCamera("myCam");
	//camAnchor->setPosition((Constants::edgeLength * Constants::gridWidth), 750, (Constants::edgeLength * Constants::gridHeight));
	camAnchor->setPosition(Ogre::Vector3::ZERO + Ogre::Vector3(847, 950, 1058));
	camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TransformSpace::TS_WORLD);
	
	mCam->setNearClipDistance(5);
	camNode->attachObject(mCam);
	Ogre::Viewport* vp = getRenderWindow()->addViewport(mCam);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	mCam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	root->addFrameListener(this);
	
	player1 = Player();
	player1.playerId = 1;
	player2 = Player();
	player2.playerId = 2;
	PlayerRelationship* relationship = new PlayerRelationship(1, PlayerRelationshipStatus::HOSTILE);
	player1.relationship = relationship;
	player2.relationship = relationship;

	activePlayer = &player1;
	players.push_back(&player1);
	players.push_back(&player2);

	//gridMap = new std::vector<std::vector<GridSquare*>>();

	mScnMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	//mScnMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
	spotLight = mScnMgr->createLight("SpotLight");
	spotLight->setType(Ogre::Light::LT_DIRECTIONAL);
	spotLight->setDirection(Ogre::Vector3(1.0f, -1.0f, -1.0f).normalisedCopy());
	spotLight->setDiffuseColour(1.0f, 1.0f, 1.0f);
	spotLight->setSpecularColour(0.25f, 0.25f, 0.25f);

	/*spotLight = mScnMgr->createLight("SpotLight");
	spotLight->setDiffuseColour(0, 0, 1.0);
	spotLight->setSpecularColour(0, 0, 1.0);
	spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
	Ogre::SceneNode* spotLightNode = mScnMgr->getRootSceneNode()->createChildSceneNode();
	spotLightNode->attachObject(spotLight);
	spotLightNode->setDirection(-1, -1, 0);
	spotLightNode->setPosition(Ogre::Vector3(200, 200, 0));
	spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));*/

	mRayScnQuery = mScnMgr->createRayQuery(Ogre::Ray());
	mRayScnQuery->setSortByDistance(true);

	/* TESTING ZONE */

	//Terrain


	/*Ogre::MaterialPtr mat =
		Ogre::MaterialManager::getSingleton().create(
			"PlaneMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	Ogre::TextureUnitState* tuisTexture =
		mat->getTechnique(0)->getPass(0)->createTextureUnitState("grass_1024.jpg");

	Ogre::MovablePlane* mPlane = new Ogre::MovablePlane("Plane");
	mPlane->d = 0;
	mPlane->normal = Ogre::Vector3::UNIT_Y;

	Ogre::MeshManager::getSingleton().createPlane(
		"PlaneMesh",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		*mPlane,
		1000, 1000, 1, 1,
		true,
		1, 1, 1,
		Ogre::Vector3::UNIT_Z);
	Ogre::Entity* mPlaneEntity = mScnMgr->createEntity("PlaneMesh");
	mPlaneEntity->setMaterialName("PlaneMat");

	Ogre::SceneNode* mPlaneNode = mScnMgr->getRootSceneNode()->createChildSceneNode();
	mPlaneNode->attachObject(mPlaneEntity);*/

	Ogre::Plane oceanSurface;
	oceanSurface.normal = Ogre::Vector3::UNIT_Y;
	oceanSurface.d = 20;
	Ogre::MeshManager::getSingleton().createPlane("OceanSurface",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		oceanSurface,
		500, 500, 50, 50, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* mOceanSurfaceEnt = mScnMgr->createEntity("OceanSurface", "OceanSurface");
	mScnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mOceanSurfaceEnt);
	mOceanSurfaceEnt->setMaterialName("Examples/WaterStream");
	mOceanSurfaceEnt->getParentSceneNode()->translate(1200, 195, 1120);
	
	selectBox = new SelectionBox("SelectionBox");
	mScnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(selectBox);

	volQuery = mScnMgr->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList());
	gridEditor = new GridEditor(mScnMgr, &gridMap);

	//Box2D
	//b2Vec2 gravity(0.0f, -10.0f);
	b2Vec2 gravity(0.0f, 0.0f);
	mWorld = new b2World(gravity);

	Ogre::Vector3 buildingPosition = GridUtils::numericalCordFinder(8, 5);
	buildingPosition.y += Constants::unitBaseHeight;
	Ogre::String buildingName = "RedBrickBuilding";
	mRedBuilding = new Building(mScnMgr, buildingPosition, buildingName, "RedBrickShack.mesh", mWorld, &impassableTerrain);
	mRedBuilding->setSpawnPoint(Ogre::Vector2(8, 7));
	activePlayer->myBuildings.insert(std::make_pair(buildingName, mRedBuilding));


	

	//treeEntity->setCastShadows(true);
	/*Ogre::SceneNode* treeNode = mScnMgr->getRootSceneNode()->createChildSceneNode("tree", Ogre::Vector3(560, 200, 640));
	treeNode->rotate(Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Y));
	treeNode->setScale(50, 50, 50);
	treeNode->attachObject(treeEntity);*/

	//TODO move impassableTerrain, gridEditor, and b2 world to the map manager (probably?)
	mMapManager = new MapManager(mScnMgr, spotLight, &impassableTerrain, mWorld, gridEditor);

	mUnitController = new UnitController(&units, &players, &projectiles, &magicAttacks);
	//GenerateUnits::generateOneBronze(mScnMgr, &units, &player1.myArmy, mWorld);
	//GenerateUnits::generateFourBronze(mScnMgr, &units, &player1.myArmy, mWorld);
	//GenerateUnits::generateFourBronze(mScnMgr, &units, &player1.myArmy);
	GenerateUnits::generateEightBronze(mScnMgr, &units, &player1.myArmy, mWorld, &impassableTerrain, mUnitController);
	GenerateUnits::generateTrees(mScnMgr, &natResources, mWorld, &impassableTerrain);

	Ogre::String unit8675 = "UnitNode" + Ogre::StringConverter::toString(8675309);

	Constants constants;
	Ogre::String peasant = constants.peasant;
	Unit* unit = new Villager(mScnMgr, GridUtils::numericalCordFinder(0, 6), unit8675, peasant, "Villager", 8675309, mWorld, &impassableTerrain, mUnitController);
	unit->currentPos = Ogre::Vector2(0, 6);
	unit->mPlayerId = 1;
	units.insert(std::make_pair(unit8675, unit));
	player1.myArmy.insert(std::pair<Ogre::String, Unit*>(unit8675, unit));

	/** wield an axe **/
	/*Ogre::Vector3 peasantPos = GridUtils::numericalCordFinder(3, 6);
	peasantPos.y += 200;
	Ogre::Entity* axeEntity = mScnMgr->createEntity("Axe.mesh");*/
	/*Ogre::SceneNode* axeNode = mScnMgr->getRootSceneNode()->createChildSceneNode("axe", peasantPos);
	axeNode->setScale(50, 50, 50);
	axeNode->attachObject(axeEntity);*/

	/*Ogre::Entity* unitEntity = mScnMgr->createEntity(peasant);
	Ogre::SceneNode* unitNode = mScnMgr->getRootSceneNode()->createChildSceneNode("Peasant", peasantPos);
	unitNode->setScale(50, 50, 50);
	unitNode->attachObject(unitEntity);*/

	//Ogre::Quaternion rot = Ogre::Quaternion::IDENTITY;  // - could make this configurable
	//Ogre::Quaternion rotation = Ogre::Quaternion(0, 1, sqrt(0.5), sqrt(0.5));
	//Ogre::Quaternion rotation = Ogre::Quaternion(0, sqrt(0.5), 1, sqrt(0.5));
	//Ogre::Vector3 offset = Ogre::Vector3(-0.45, 0.35, -0.65);       // - could make this configurable
	//Ogre::Vector3 offset = Ogre::Vector3::ZERO;
	//Ogre::TagPoint* pTagPoint = unit->unitEntity->attachObjectToBone("Hand.Hold.L", axeEntity, rotation, offset);
	//Ogre::TagPoint* pTagPoint = unitEntity->attachObjectToBone("Hand.Hold.L", axeEntity, rotation, offset);

	//pTagPoint->setInheritOrientation(true);   // - could make this configurable
	//pTagPoint->setInheritParentEntityOrientation(true);   // - could make this configurable

	//unit->animate("Chopping");

	//GenerateUnits::generateOneSky(mScnMgr, &units, &player2.myArmy, mWorld, &impassableTerrain, mUnitController);
	//GenerateUnits::generateFourSky(mScnMgr, &units, &player2.myArmy, mWorld, &impassableTerrain, mUnitController);

	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 10;
	int32 positionIterations = 8;

	

	/* END TESTING ZONE */

	/** Initialize Singletons **/
	
	pim = new PlayerInputManager(mCam, volQuery, mScnMgr);
	GameMgr = new GameManager();
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

void GameRunnable::spawnProjectile(Unit* unit) {
	projectiles.push_back(new Projectile(unit, mScnMgr));
}
//----------------------------------------------------------------

void GameRunnable::spawnMagic(Unit* unit) {
	magicAttacks.push_back(new MagicAttack(unit, mScnMgr));
}
//----------------------------------------------------------------

void GameRunnable::clearTargets(Unit* expiredTarget) {
	for (std::map<Ogre::String, Unit*>::iterator it = units.begin(); it != units.end(); ++it) {
		Unit* unit = it->second;
		if (unit->mTarget == expiredTarget) {
			unit->resetTarget();
		}
	}
}
//----------------------------------------------------------------

bool GameRunnable::doesUnitExist(int unitId) {
	for (auto it = units.begin(); it != units.end(); ++it) {
		Unit* unit = it->second;
		if (unit->unitID == unitId)
			return true;
	}

	return false;
}


void GameRunnable::frameRendered(const Ogre::FrameEvent& evt)
{
	/* Camera movement */
	// Working code but annoying when trying to debug
	if (mMoveableCamera->isChecked()) {
		Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
		int mMoveScale = 250;
		int mHalfScale = 125;
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (x > (getRenderWindow()->getWidth() - 20)) {
			transVector.x += mMoveScale;
			transVector.z -= mMoveScale;
		}
		else if (x < 20) {
			transVector.x -= mMoveScale;
			transVector.z += mMoveScale;
		}
		else if (y > (getRenderWindow()->getHeight() - 20)) {
			transVector.z += mMoveScale;
			transVector.x += mMoveScale;
			//transVector.y -= mMoveScale;
		}
		else if (y < 20) {
			transVector.z -= mMoveScale;
			transVector.x -= mMoveScale;
			//transVector.y += mMoveScale;
		}
		//mScnMgr->getSceneNode("camNode")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		mScnMgr->getSceneNode("camAnchor")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	}

	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 10;
	int32 positionIterations = 8;
	mWorld->Step(timeStep, velocityIterations, positionIterations);
	mWorld->ClearForces();

	/** Projectile animation frame rendering **/
	
	for (std::vector<Projectile*>::iterator projectile = projectiles.begin(); projectile != projectiles.end(); ++projectile) {
		if ((*projectile)->mDistance > 0.0) {
			Ogre::Real move = (*projectile)->mWalkSpeed * evt.timeSinceLastFrame;
			(*projectile)->mDistance -= move;

			(*projectile)->projectileNode->translate(move * (*projectile)->mDirection);
		}
		else {
			(*projectile)->dealDamage();
			Ogre::String objName = (*projectile)->projectileName;
			mScnMgr->destroySceneNode(objName);
			projectiles.erase(projectile--);
		}
	}

	/** Particle animation frame rendering **/

	for (std::vector<MagicAttack*>::iterator magic = magicAttacks.begin(); magic != magicAttacks.end(); ++magic) {
		if ((*magic)->summoner->unitAnimState->hasEnded() || (*magic)->summoner->mTarget == NULL) {
			(*magic)->dealDamage();
			Ogre::String objName = (*magic)->particleName;
			mScnMgr->destroySceneNode(objName);
			magicAttacks.erase(magic--);
		}
	}

	/** Unit animation frame rendering **/

	for (auto player = players.begin(); player != players.end(); ++player) {
		for (std::map<Ogre::String, Unit*>::iterator it = (*player)->myArmy.begin(); it != (*player)->myArmy.end(); ++it) {
			Unit* unit = it->second;

			//TODO remove references to units from GameRunnable
			//TODO Player should be renamed to player and some of its functionality should be extracted to an actual player manager
			//TODO Units should become a child class of a generic GameObject class
			//TODO implement unit generation

			unit->update(evt);
		}
	}

	mTrayMgr->frameRendered(evt);

	/** Clear units from the simulation **/
	// This must be at the end of the step otherwise B2 will throw an exception
	for (std::map<Ogre::String, Unit*>::iterator mortality = units.begin(); mortality != units.end(); ++mortality) {
		Unit* mortalUnit = mortality->second;
		/** Remove units with zero hitpoints **/
		if (mortalUnit->mHitPoints <= 0) {
			units.erase(mortality--);

			/** ensure no one is targeting the dieing unit **/
			mortalUnit->mUnitController->clearTargets(mortalUnit);

			/** cull the dead unit **/
			for (auto player = players.begin(); player != players.end(); ++player) {
				if ((*player)->hasUnitInArmy(mortalUnit->unitName)) {
					(*player)->cullUnit(mortalUnit);
				}
			}
			delete mortalUnit;
		}
	}
}
//----------------------------------------------------------------


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