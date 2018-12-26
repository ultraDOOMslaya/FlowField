#include "GameRunnable.h"

GameRunnable::GameRunnable()
	: OgreBites::ApplicationContext("SeaWolves"),
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
}
//----------------------------------------------------------------

void GameRunnable::createSquare(int width, int height, int edgeLength, std::string meshName, bool oddOrEven, Ogre::ColourValue color)
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
			createSquare(xscale, yscale, Constants::edgeLength, strToPass, true, (Ogre::ColourValue::Blue));
		}
		else {
			createSquare(xscale, yscale, Constants::edgeLength, strToPass, false, (Ogre::ColourValue::ColourValue(0.2f, 0.2f, 1.0f, 1.0f)));
		}

		if (temp == 0) {
			mat = mScnMgr->getEntity("0")->getSubEntity(0)->getMaterial()->clone("BaseRedNoLighting");
			Ogre::TextureUnitState* texture = new Ogre::TextureUnitState(mat->getTechnique(0)->getPass(0));
			mat->getTechnique(0)->getPass(0)->addTextureUnitState(texture);
			mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue::Red);
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

void swap(float& x, float& y)
{
	float temp = x;
	x = y;
	y = temp;
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

PlayerRelationshipStatus determineStatus(PlayerManager* activePlayer, std::vector<PlayerManager*> players, Unit* unit) {
	if (activePlayer->hasUnitInArmy(unit->unitName)) {
		return PlayerRelationshipStatus::ME;
	}
	else {
		for (std::vector<PlayerManager*>::iterator it = players.begin(); it < players.end(); it++) {
			PlayerManager* player = *it;
			if (player->hasUnitInArmy(unit->unitName)) {
				if (player->relationship->isFoe()) {
					return PlayerRelationshipStatus::HOSTILE;
				}
			}
		}
	}
	return PlayerRelationshipStatus::FRIENDLY;
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
						gridEditor->addTerrainValue(it->movable->getName(), mat, &impassableTerrain);
					}
				}
			}
			else if (result.size() > 1) {		//Found the floor object AND something else
				Unit* unit;
				for (; it != result.end(); it++) {
					if (it->movable->getQueryFlags() == Constants::unitQueryMask) {
						std::map<Ogre::String, Unit*>::iterator itTree = units.find(it->movable->getParentSceneNode()->getName());
						unit = itTree->second;
						activePlayer->focusUnit(unit);
						break;
					}
				}
			}
			else {								//Just the floor object
				activePlayer->clearUnitQueue();
			}
		}
		else {
			performSelection(selectBox->mStart, selectBox->mStop);
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
					switch (determineStatus(activePlayer, players, unit)) 
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
		PathFinding* path = new PathFinding(SquareIndex, &impassableTerrain, mScnMgr);
		if (mShowFlowPathCB->isChecked()) {
			path->showFlow(mScnMgr);
		}

		// TODO refactor to allow an active player and unit movement for multiple players
		//for(std::map<Ogre::String, Unit>::iterator it = units.begin(); it != units.end(); ++it) {
		for (std::vector<Unit*>::iterator unit = activePlayer->unitQueue.begin(); unit != activePlayer->unitQueue.end(); ++unit) {

			//TODO temporary for flocking work. Remove or refactor. Would like to see a single method with code the render loop uses.
			Ogre::Vector2 aPos = GridUtils::numericalCordFinder(path->flowField[(*unit)->currentPos.x][(*unit)->currentPos.y]);
			Ogre::Vector3* nextCord = new Ogre::Vector3(aPos.x, 0, aPos.y);
			(*unit)->walkList.push_back(*nextCord);

			Ogre::Vector2 finalPosition = GridUtils::numericalCordFinder(SquareIndex);
			Ogre::Vector3* finalCord = new Ogre::Vector3(finalPosition.x, 0, finalPosition.y);
			(*unit)->finalDestination = *finalCord;

			/* Reset all unit behaviours */
			(*unit)->halt();
			(*unit)->attacking = false;

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


void GameRunnable::performSelection(
	const Ogre::Vector2& first,
	const Ogre::Vector2& second)
{
	float left = first.x, right = second.x;
	float top = first.y, bottom = second.y;

	if (left > right)
		swap(left, right);

	if (top > bottom)
		swap(top, bottom);

	if ((right - left) * (bottom - top) < 0.0001)
		return;

	Ogre::Ray topLeft = mCam->getCameraToViewportRay(left, top);
	Ogre::Ray topRight = mCam->getCameraToViewportRay(right, top);
	Ogre::Ray bottomLeft = mCam->getCameraToViewportRay(left, bottom);
	Ogre::Ray bottomRight = mCam->getCameraToViewportRay(right, bottom);

	Ogre::Plane frontPlane, topPlane, leftPlane, bottomPlane, rightPlane;

	frontPlane = Ogre::Plane(
		topLeft.getOrigin(),
		topRight.getOrigin(),
		bottomRight.getOrigin());

	topPlane = Ogre::Plane(
		topLeft.getOrigin(),
		topLeft.getPoint(10),
		topRight.getPoint(10));

	leftPlane = Ogre::Plane(
		topLeft.getOrigin(),
		bottomLeft.getPoint(10),
		topLeft.getPoint(10));

	bottomPlane = Ogre::Plane(
		bottomLeft.getOrigin(),
		bottomRight.getPoint(10),
		bottomLeft.getPoint(10));

	rightPlane = Ogre::Plane(
		topRight.getOrigin(),
		topRight.getPoint(10),
		bottomRight.getPoint(10));

	Ogre::PlaneBoundedVolume vol;

	vol.planes.push_back(frontPlane);
	vol.planes.push_back(topPlane);
	vol.planes.push_back(leftPlane);
	vol.planes.push_back(bottomPlane);
	vol.planes.push_back(rightPlane);

	Ogre::PlaneBoundedVolumeList volList;
	volList.push_back(vol);

	volQuery->setVolumes(volList);
	Ogre::SceneQueryResult& result = volQuery->execute();

	//activePlayer->focusUnits(result, &units);
	activePlayer->focusUnits(result);
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

	//GenerateUnits::generateOneBronze(mScnMgr, &units);
	GenerateUnits::generateFourBronze(mScnMgr, &units, &player1.myArmy);
	//GenerateUnits::generateEightBronze(mScnMgr, &units);

	//GenerateUnits::generateOneSky(mScnMgr, &units, &player2.myArmy);
	GenerateUnits::generateFourSky(mScnMgr, &units, &player2.myArmy);
	
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
	gridEditor = new GridEditor(mScnMgr);

	/* END TESTING ZONE */
}
//----------------------------------------------------------------

int pythagoreanCalculation(int a, int b) {
	int cSquared = (a * a) + (b * b);
	int c = std::sqrt(cSquared);
	return c;
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

Ogre::Vector3 GameRunnable::staticObjectCollisionForceApplier(Unit* unit) {
	Ogre::Vector3 totalForce = Ogre::Vector3(0, 0, 0);

	unit->path->squareNeighbors = GridUtils::getTerrainNeighbors(unit->currentPos, unit->path->squareNeighbors, &unit->path->dijkastraGrid);
	//getAllNeighbors(*unit);

	int terrainBoundry = ((Constants::edgeLength * 0.7) / 2) + unit->seperationRadius;
	int neighborsCount = 0;
	int x = 0;
	int z = 0;
	bool xMod, zMod;

	while (!unit->path->squareNeighbors.empty()) {
		SquareNeighbor* neighbor = unit->path->squareNeighbors.front();
		if (neighbor->getDistance() == Constants::WALL) {
			Ogre::Vector2 cords = GridUtils::numericalCordFinder(neighbor->getPosition());
			Ogre::Vector3 terrain(cords.x, 0, cords.y);
			int a, b;
			a = std::abs(terrain.x - unit->getPosition().x);
			b = std::abs(terrain.z - unit->getPosition().z);
			// Utilizing Pythagorean theorm:
			// *** Terrain ***
			
			int terrainHypotenuse = pythagoreanCalculation(terrainBoundry, terrainBoundry);
			int unitHypotenuse = pythagoreanCalculation(a, b);

			if (unitHypotenuse < terrainHypotenuse) {
				Ogre::Vector3* pushForce = subtractVector(unit->getPosition(), terrain);

				totalForce.operator+=(pushForce->operator/(unit->seperationRadius));

				unit->debugPos2 = Ogre::Vector2(totalForce.x, totalForce.z);

				/*
				if (unit->currentPos.x == neighbor->getPosition().x) {
					totalForce.x = 0;
				}
				else if (unit->currentPos.y == neighbor->getPosition().y) {
					totalForce.y = 0;
				}
				else {
					totalForce.y = 0;
				}
				*/

				neighborsCount++;
				free(pushForce);
			}
		}
		free(neighbor);
		unit->path->squareNeighbors.pop();
	}

	if (neighborsCount == 0) {
		return totalForce;
	}

	totalForce.operator/=(neighborsCount);
	totalForce.operator*=(unit->maxForce);

	for (std::vector<Unit*>::iterator it = unit->group->begin(); it != unit->group->end(); ++it) {
		Unit* u = (*it);
		if (u->unitName != unit->unitName) {
			int distance = distanceTo(unit->getPosition(), u->getPosition());

			if (distance < (unit->maxCohesion / 2)) {
				totalForce.operator+=(totalForce);
			}
		}
	}

	/*if (xMod) {
		unit->forceToApply.x = 0;
		unit->velocity.x = 1;
	}

	if (zMod) {
		unit->forceToApply.z = 0;
		unit->velocity.z = 1;
	}*/

	return totalForce;
}


void GameRunnable::frameRendered(const Ogre::FrameEvent& evt)
{
	/* Camera movement */
	// Working code but annoying when trying to debug
	/*Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
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
	mScnMgr->getSceneNode("camNode")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);*/
	

	for (std::map<Ogre::String, Unit*>::iterator it = units.begin(); it != units.end(); ++it) {
		Unit* unit = it->second;

		if (unit->attacking) {
			if (!unit->attackTarget()) {
				unit->distanceFromTarget = unit->seekTargetRadius;
				for (std::map<Ogre::String, Unit*>::iterator it = units.begin(); it != units.end(); ++it) {
					Unit* potentialTarget = it->second;
					int distance = distanceTo(unit->getPosition(), potentialTarget->getPosition());
					if (distance < unit->seekTargetRadius && distance < unit->distanceFromTarget) {
						if (determineStatus(activePlayer, players, potentialTarget) == PlayerRelationshipStatus::HOSTILE) {
							unit->mTarget = potentialTarget;
							unit->distanceFromTarget = distance;
						}
					}
				}
			}
		}

		if (unit->unitAnimState->getAnimationName() == "Walk") {
		//if (unit->unitAnimState->getAnimationName() == "Run") {
		//if (unit->nextLocation()) {
			
			/* Realtime directing */
		
			Ogre::Vector2 position = GridUtils::cordNumericalFinder(unit->getPosition());
			if (position != unit->currentPos) {
				unit->currentPos = position;
			}

			Ogre::Vector3 cords = Ogre::Vector3::ZERO;
			if (unit->hasLos()) {
				cords = unit->finalDestination;
			}
			else {
				Ogre::Vector2 direction = GridUtils::numericalCordFinder(*unit->getCurrentFlowValue());
				int x, y;
				x = direction.x;
				y = direction.y;
				cords = Ogre::Vector3(x, 0, y);
			}
			
			/* Physics applied to everyone */
			unit->debugPos2 = Ogre::Vector2(cords.x, cords.z);

			Ogre::Vector3 seek = SteeringBehaviour::seek(unit, cords);
			Ogre::Vector3 seperation = SteeringBehaviour::seperation(unit, &units);

			/* Physics applied to groups */
			Ogre::Vector3 cohesion = SteeringBehaviour::cohesion(unit, unit->group);
			Ogre::Vector3 alignment = SteeringBehaviour::alignment(unit, unit->group);
			Ogre::Vector3 collision = staticObjectCollisionForceApplier(unit);
			//collision.operator*=(2);

			//unit->forceToApply = seek.operator*=(0.75).operator+=(seperation.operator*=(15)).operator+=(cohesion.operator*=(0.5)).operator+=(alignment.operator/=(3)).operator+=(collision.operator*=(3));
			//unit->forceToApply = seek.operator+=(seperation.operator*=(10)).operator+=(cohesion.operator*=(0.1)).operator+=(alignment).operator+=(collision);
			//unit->forceToApply = seek.operator+=(seperation.operator*=(10)).operator+=(cohesion.operator*=(0.1)).operator+=(alignment);
			unit->forceToApply = seek.operator*=(2).operator+=(seperation.operator*=(30)).operator+=(cohesion.operator*=(0.1)).operator+=(alignment.operator*=(2)).operator+=(collision);

			//Ogre::Vector3 collision = staticObjectCollisionForceApplier(unit);
			SteeringBehaviour::halt(unit, unit->group);
		}
	}

	
	Ogre::AnimationState* tempState;
	for (std::map<Ogre::String, Unit*>::iterator it = units.begin(); it != units.end(); ++it) {
		Unit* unit = it->second;
	
		if (unit->nextLocation()) {
			unit->animate("Walk");
			unit->velocity = unit->direction;
		}
		if (unit->destination != Ogre::Vector3::ZERO) {
			//unit->velocity.operator+=(unit->forceToApply.operator*=(evt.timeSinceLastFrame));		//Delta Time
			//unit->velocity.operator+=(unit->forceToApply.operator*=(0.0166666));					//Mocked Delta Time
			unit->velocity.operator+=(unit->forceToApply.operator*=(0.00166666));					//Mocked Delta Time
			float speed = unit->velocity.length();
			if (speed > unit->maxSpeed) {
				unit->velocity.operator*=(unit->maxSpeed / speed);
			}

			unit->rotate(unit->velocity);

			
			//Ogre::Vector3 newPos = unit->getPosition().operator+=(unit->velocity.operator*(0.0166666));
			Ogre::Vector3 newPos = unit->getPosition().operator+=(unit->velocity.operator*(0.00166666));
			//Ogre::Vector3 newPos = unit->getPosition().operator+=(unit->velocity.operator*(evt.timeSinceLastFrame));
			unit->commandMove(newPos);
		}
		unit->unitAnimState->addTime(evt.timeSinceLastEvent);		
	}
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