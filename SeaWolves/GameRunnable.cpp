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
	selectBox->checkAndUpdate(getRenderWindow()->getWidth(), getRenderWindow()->getHeight());

	return true;
}
//----------------------------------------------------------------

bool GameRunnable::mouseReleased(const OgreBites::MouseButtonEvent &evt)
{
	OgreBites::Event fullEvt = OgreBites::Event();
	fullEvt.button = evt;
	selectBox->disable();

	Ogre::String objectName = "";
	Ogre::Ray mouseRay = mTrayMgr->getCursorRay(mCam);

	mRayScnQuery->setRay(mouseRay);
	Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
	Ogre::RaySceneQueryResult::iterator it = result.begin();
	int rayQuerySize = result.size();

	if (evt.button == SDL_BUTTON_LEFT) {
		// Did we drag or did we click?
		if (selectBox->mStop == selectBox->mStart) {
			if (rayQuerySize > 1) {
				//Found the floor object AND something else
				Unit* unit; //TODO eventually we will have buildings and resources. Make this a gameobject and have unit inherit from it.
				for (; it != result.end(); it++) {
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
			}
		}
		else {
			pim->performSelection(selectBox->mStart, selectBox->mStop, activePlayer, gridMap);
		}
		selectBox->disable();
	}

	if (evt.button == SDL_BUTTON_RIGHT) {

		if (result.size() == 0)
			return true;

		for (; it != result.end(); it++)
		{
			objectName = it->movable->getName();
		}
		// Used for right click movement... basic click to move
		Ogre::Vector2 SquareIndex = GridUtils::gridIndexFinder(objectName);

		//cordinate debugging
		std::ostringstream oss;
		oss << objectName;
		mCordPanel->setText(oss.str());

		PathFinding* path = new PathFinding(SquareIndex, &impassableTerrain, mScnMgr, activePlayer->unitQueue.size(), activePlayer->unitGroupConglomerate());

		//TODO move this to a UI manager that path will then check on creation
		if (mShowFlowPathCB->isChecked()) {
			path->showFlow(mScnMgr);
		}

		pim->clearFocusedLocations(activePlayer, gridMap);

		for (std::vector<Unit*>::iterator unit = activePlayer->unitQueue.begin(); unit != activePlayer->unitQueue.end(); ++unit) {
			(*unit)->handleInput(fullEvt);
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
		}

		//TODO this may be the wrong place for this after the refactor
		/* Reset state player state */
		//activePlayer->queuedAttackMove = false;

		if (activePlayer->unitQueue.size() > 0) {
			for (std::vector<Ogre::Vector2*>::iterator it = path->formationLocations.begin(); it != path->formationLocations.end(); ++it) {
				Ogre::Vector2* point = *it;
				mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue::Green);

				gridEditor->changeTileColor(point->x, point->y, mat, Ogre::ColourValue::Green);

			}
		}

		if (!mNonCombat->isChecked()) {
			Ogre::RaySceneQueryResult::iterator it = result.begin();
			// Right click on an enemy player?
			if (rayQuerySize > 1) {
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
				}
			}
		}
		
	}

	if (evt.button == SDL_BUTTON_LEFT) {
		//Editor stuff
		if (mModeCB->isChecked()) {
			Ogre::String squareName;
			for (; it != result.end(); it++) {
				if (it->movable->getQueryFlags() == Constants::terrainQueryMask) {
					gridEditor->addTerrainValue(it->movable->getName(), redMat, mWorld, &impassableTerrain);
				}
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
	mCordPanel = mTrayMgr->createTextBox(OgreBites::TL_BOTTOMRIGHT, "MouseCords", "Mouse Cordinates", 175.0f, 125.0f);
	//mModeWidget = mTrayMgr->createParamsPanel(OgreBites::TL_TOPRIGHT, "ModeSet",  Ogre::Real(175), 3);
	mModeCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "WallGen", "Editor Mode", 150.0f);
	mUnitCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "UnitGen", "New Units", 150.0f);
	mShowFlowPathCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "ShowPath", "Show Paths", 150.0f);
	mNonCombat = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "NonCombat", "Non Combat", 150.0f);
	mMoveableCamera = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "MoveableCamera", "Moveable Camera", 150.0f);

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
	//GenerateUnits::generateOneBronze(mScnMgr, &units, &player1.myArmy, mWorld);
	//GenerateUnits::generateFourBronze(mScnMgr, &units, &player1.myArmy, mWorld);
	//GenerateUnits::generateFourBronze(mScnMgr, &units, &player1.myArmy);
	GenerateUnits::generateEightBronze(mScnMgr, &units, &player1.myArmy, mWorld, &impassableTerrain);

	//GenerateUnits::generateOneSky(mScnMgr, &units, &player2.myArmy, mWorld);
	GenerateUnits::generateFourSky(mScnMgr, &units, &player2.myArmy, mWorld, &impassableTerrain);

	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 10;
	int32 positionIterations = 8;

	/* END TESTING ZONE */

	/** Initialize Singletons **/
	mUnitController = UnitController();
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
	projectiles.push_back(Projectile(unit, mScnMgr));
}
//----------------------------------------------------------------

void GameRunnable::spawnMagic(Unit* unit) {
	magicAttacks.push_back(MagicAttack(unit, mScnMgr));
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
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (x > (getRenderWindow()->getWidth() - 20)) {
			transVector.x += mMoveScale;
		}
		else if (x < 20) {
			transVector.x -= mMoveScale;
		}
		else if (y > (getRenderWindow()->getHeight() - 20)) {
			transVector.y -= mMoveScale;
		}
		else if (y < 20) {
			transVector.y += mMoveScale;
		}
		mScnMgr->getSceneNode("camNode")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	}

	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 10;
	int32 positionIterations = 8;
	mWorld->Step(timeStep, velocityIterations, positionIterations);
	mWorld->ClearForces();

	for (auto player = players.begin(); player != players.end(); ++player) {
		for (std::map<Ogre::String, Unit*>::iterator it = (*player)->myArmy.begin(); it != (*player)->myArmy.end(); ++it) {
			Unit* unit = it->second;

			switch (unit->mState) {
			case Unit::STATE_IDLE:
				break;
			case Unit::STATE_MARCHING:
				break;
			case Unit::STATE_FINISH_JOURNEY:
				//mUnitController.proximityLocationFormation(unit->path->origin.x, unit->path->origin.y, activePlayer->unitQueue, unit->path);
				break;
			case Unit::STATE_ATTACKING:
				break;
			case Unit::STATE_HUNTING:
				CombatBehaviour::huntForTarget(&units, (*player), players, unit);
				break;
			case Unit::STATE_SEEKING:
				CombatBehaviour::seekTarget(&units, (*player), players, unit);
				break;
			}

			/** This will throw exceptions, if placed near the top, if dealing damage to an already dead unit **/

			//TODO fix unit stuttering at sudden direction changes
			//TODO remove references to units from GameRunnable
			//TODO Player should be renamed to player and some of its functionality should be extracted to an actual player manager
			//TODO Units should become a child class of a generic GameObject class
			//TODO implement unit generation

			/** Attack Code **/
			/** Idle, Attacking, or Hunting **/
			if (unit->mState == Unit::STATE_AGGRESSIVE) {
				if (unit->hasTarget()) {
					unit->distanceFromTarget = unit->getPosition().squaredLength() - unit->mTarget->getPosition().squaredLength();
					if (unit->inRange()) {
						unit->mState = Unit::STATE_ATTACKING;
					}
					else {
						if (unit->unitAnimState->getAnimationName() != "Walk") {
							unit->walkList.push_back(unit->mTarget->getPosition());
						}
						else {
							//Assign units a final destination but give them a *dummy path as well
							unit->finalDestination = unit->mTarget->getPosition();
							unit->b2FinalDestination = unit->mTarget->getB2DPosition();
						}
					}
				}
				else if (unit->isHunting()) {
					unit->mState = Unit::STATE_HUNTING;
				}
				else {
					unit->mState = Unit::STATE_SEEKING;
				}
			}

			if (unit->mState == Unit::STATE_POST_COMBAT) {
				if (unit->hasTarget() && unit->inRange()) {
					unit->mState = Unit::STATE_ATTACKING;
				}
				else {
					Ogre::Vector2 aPos = GridUtils::numericalCordFinder(unit->path->flowField[unit->currentPos.x][unit->currentPos.y]);
					b2Vec2 nextCord = b2Vec2(aPos.x, aPos.y);
					unit->b2WalkList.push_back(nextCord);
					unit->b2FinalDestination = unit->postCombatB2Desination;
					unit->mState = Unit::STATE_AGGRESSIVE;
				}
			}

			if (unit->mState == Unit::STATE_ATTACKING) {
				unit->attack();
				//TODO make this a method in Unit
				if (unit->mUnitClass == "HeavyArmor" && (unit->unitAnimState->getTimePosition() > (unit->unitAnimState->getLength() * 0.5))) {

					if (!unit->hasAttacked)
						unit->mTarget->takeDamage(unit->mAttackDamage);

					unit->hasAttacked = true;
				}
				if (unit->mUnitClass == "Fletcher" && (unit->unitAnimState->getTimePosition() < unit->animationElapsedTime)) {
					if (!unit->hasAttacked) {
						//unit->spellActionQueue.push(unit);
						//CombatBehaviour::spawnSpellAction(unit->spellActionQueue.front(), &projectiles, &magicAttacks, mScnMgr);
						//unit->spellActionQueue.pop();
						spawnProjectile(unit);
						//CombatBehaviour::spawnSpellAction(unit, &projectiles, &magicAttacks, mScnMgr);
					}

					unit->hasAttacked = true;
				}
				if (unit->mUnitClass == "Caster" && (unit->unitAnimState->getTimePosition() > (unit->unitAnimState->getLength() * 0.8))) {
					if (!unit->hasAttacked) {
						/*unit->spellActionQueue.push(unit);
						CombatBehaviour::spawnSpellAction(unit->spellActionQueue.front(), &projectiles, &magicAttacks, mScnMgr);
						unit->spellActionQueue.pop();*/
						spawnMagic(unit);
						//CombatBehaviour::spawnSpellAction(unit, &projectiles, &magicAttacks, mScnMgr);
					}

					unit->hasAttacked = true;
				}
				unit->animationElapsedTime = unit->unitAnimState->getTimePosition();
			}

			/** Kinetic unit states **/
			if (unit->mState == Unit::STATE_MARCHING || unit->mState == Unit::STATE_AGGRESSIVE || unit->mState == Unit::STATE_HUNTING || unit->mState == Unit::STATE_SEEKING) {

				/* Realtime directing */
				unit->debugB2Pos1 = unit->getB2DPosition();
				Ogre::Vector2 position = GridUtils::b2CordNumericalFinder(unit->getB2DPosition());
				if (position != unit->currentPos) {
					unit->currentPos = position;
				}

				/** If the group has LOS, assign the most final destination **/
				// This assigns units into formation if we have LOS
				if (unit->mTarget == NULL) {
					if (unit->assignedPathLosDiscovered()) {
						//unit->mPreviousState = unit->mState;
						//unit->mState = Unit::STATE_FINISH_JOURNEY;
						mUnitController.proximityLocationFormation(unit->path->origin.x, unit->path->origin.y, activePlayer->unitQueue, unit->path);
						//gom->proximityLocationFormation(unit->path->origin.x, unit->path->origin.y, *(unit->group), unit->path); //was activeplayer->UnitQueue
					}
				}
				
				b2Vec2 cords;
				if (unit->hasLos()) {
					cords = unit->b2FinalDestination;
				}
				else {
					Ogre::Vector2 direction = GridUtils::numericalCordFinder(*unit->getCurrentFlowValue());
					unit->debugPos1 = *unit->getCurrentFlowValue();
					cords = GridUtils::b2NumericalCordFinder(*unit->getCurrentFlowValue());
				}

				/* Physics applied to everyone */
				unit->debugPos2 = Ogre::Vector2(cords.x, cords.y);
				b2Vec2 seek = SteeringBehaviour::seek(unit, cords);
				unit->b2ForceToApply = seek;

				//TODO put this somewhere... do it as soon as you see this!
				if (unit->hasArrived()) {
					unit->halt();
				}
			}

			/** unit subroutine **/
			if (unit->nextLocation()) {
				unit->animate("Walk");
				unit->b2Velocity = unit->b2Direction;
				unit->mBody->SetType(b2_dynamicBody);
				//NOTE this code seems to be obsolete after the implementation of POSTCOMBAT unit state
				/*if ((unit->mState != Unit::STATE_AGGRESSIVE) && (unit->mState != Unit::STATE_HUNTING) && (unit->mState != Unit::STATE_SEEKING))
					unit->mState = Unit::STATE_MARCHING;*/
			}

			/** Marching and Hunting **/
			if (unit->b2Destination != b2Vec2_zero) {

				unit->b2ForceToApply.operator*=(evt.timeSinceLastFrame);		//Real Time
				//unit->b2ForceToApply.operator*=(0.0166666);			//Mocked Delta Time
				unit->b2Velocity.operator+=(unit->b2ForceToApply);

				float speed = unit->b2Velocity.Length();
				if (speed > unit->maxSpeed) {
					unit->b2Velocity.operator*=(unit->maxSpeed / speed);
				}

				unit->rotate(unit->b2Velocity);

				//Ogre::Vector3 newPos = unit->getPosition().operator+=(unit->velocity.operator*(0.0166666));			//Mocked Delta Time
				Ogre::Vector3 newPos = unit->getPosition().operator+=(unit->velocity.operator*(evt.timeSinceLastFrame));		//Real Time

				unit->mBody->SetLinearVelocity(unit->b2Velocity);
				Ogre::Vector3 moveGraphic = Ogre::Vector3(unit->getB2DPosition().x, 0, unit->getB2DPosition().y);
				unit->debugPos2 = Ogre::Vector2(moveGraphic.x, moveGraphic.z);
				unit->commandMove(moveGraphic);

			}
			/** Attacking and Idle **/
			else {
				/** Code responsible for stopping unit movement **/
				//TODO Consider throwing this in a method
				unit->b2Velocity(0.0f);
				unit->mBody->SetLinearVelocity(b2Vec2_zero);
				unit->mBody->SetType(b2_staticBody);
				unit->trekking = false;
			}

			/** Advance Unit Animations **/
			//unit->unitAnimState->addTime(0.0166666);				//Mocked Delta Time
			unit->unitAnimState->addTime(evt.timeSinceLastEvent);		//Real Time
		}
	}

	/** Projectile animation frame rendering **/

	for (std::vector<Projectile>::iterator projectile = projectiles.begin(); projectile != projectiles.end(); ++projectile) {
		if (projectile->mDistance > 0.0) {
			Ogre::Real move = projectile->mWalkSpeed * evt.timeSinceLastFrame;
			projectile->mDistance -= move;

			projectile->projectileNode->translate(move * projectile->mDirection);
		}
		else {
			projectile->dealDamage();
			Ogre::String objName = projectile->projectileName;
			mScnMgr->destroySceneNode(objName);
			projectiles.erase(projectile--);
		}
	}

	/** Particle animation frame rendering **/

	for (std::vector<MagicAttack>::iterator magic = magicAttacks.begin(); magic != magicAttacks.end(); ++magic) {
		if (magic->summoner->unitAnimState->hasEnded() || magic->summoner->mTarget == NULL) {
			magic->dealDamage();
			Ogre::String objName = magic->particleName;
			mScnMgr->destroySceneNode(objName);
			magicAttacks.erase(magic--);
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
			CombatBehaviour::clearTargets(&units, mortalUnit);

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