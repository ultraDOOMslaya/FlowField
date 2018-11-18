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
	debugx(0),
	debugy(0),
	debugz(0)
{
}
//----------------------------------------------------------------

GameRunnable::~GameRunnable()
{
	/*
	if (mTrayMgr) delete mTrayMgr;
	if (mCam) delete mCam;
	if (constants) delete constants;

	delete mRoot;
	*/
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
			manual->colour(Ogre::ColourValue::Red);
		}
		manual->end();
		Ogre::String resourceGroup = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
		tileMesh = manual->convertToMesh(meshName);
	}
	Ogre::Entity* planeEntity = mScnMgr->createEntity(meshName, tileMesh);
	mScnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(planeEntity);
	planeEntity->setCastShadows(false);

	if (oddOrEven) {
		planeEntity->setMaterialName("Examples/Rockwall");
	}
	else {
		planeEntity->setMaterialName("Examples/Chrome");
	}
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
			createSquare(xscale, yscale, Constants::edgeLength, strToPass, true, (Ogre::ColourValue::Green));
		}
		else {
			createSquare(xscale, yscale, Constants::edgeLength, strToPass, false, (Ogre::ColourValue::Red));
		}
	}
	
}
//----------------------------------------------------------------

Ogre::Vector2 GameRunnable::gridSquareCordFinder(std::string squareName)
{
	int squareNumber = stoi(squareName);
	float fedge = Constants::edgeLength * 0.7f;
	int x = 0;
	int z = 0;
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
	x = (xcount * fedge) + (fedge * 0.5);
	z = (ycount * fedge) + (fedge * 1.5);
	Ogre::Vector2 cords(x, z);

	std::ostringstream oss;
	oss << "Object is: " << squareNumber << " X: " << xcount << " Y: " << ycount;
	//mCordPanel->setText(oss.str());
	return cords;
}
//----------------------------------------------------------------

Ogre::Vector2 GameRunnable::numericalCordFinder(Ogre::Vector2 cordinates) {
	int x = 0;
	int y = 0;
	float fedge = Constants::edgeLength * 0.7f;
	x = (cordinates.x * fedge) + (fedge * 0.5);
	y = (cordinates.y * fedge) + (fedge * 1.5);
	Ogre::Vector2 cords(x, y);
	return cords;
}
//----------------------------------------------------------------

Ogre::Vector3 GameRunnable::numericalCordFinder(Ogre::Vector3 cordinates) {
	int x = 0;
	int z = 0;
	float fedge = Constants::edgeLength * 0.7f;
	x = (cordinates.x * fedge) + (fedge * 0.5);
	z = (cordinates.z * fedge) + (fedge * 1.5);
	Ogre::Vector3 cords(x, 0, z);
	return cords;
}
//----------------------------------------------------------------

/* Converts game position into square cordinates
*/
Ogre::Vector2 GameRunnable::cordNumericalFinder(Ogre::Vector3 position) {
	int x = 0;
	int z = 0;
	float fedge = Constants::edgeLength * 0.7f;
	x = (position.x - (fedge * 0.5)+1) / fedge;		//+1 is a round up to help the int division math
	z = (position.z - (fedge * 1.5)+1) / fedge;		//otherwise end up with cases where unit movement falls short
	Ogre::Vector2 cords(x, z);
	return cords;
}
//----------------------------------------------------------------

Ogre::Vector2 GameRunnable::gridIndexFinder(Ogre::String squareName) {
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
	Ogre::Vector2 squareIndex = Ogre::Vector2(xcount, ycount);
	return squareIndex;
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
	return true;
}
//----------------------------------------------------------------

bool GameRunnable::mouseMoved(const OgreBites::MouseMotionEvent &evt)
{
	//mTrayMgr->refreshCursor();
	return true;
}
//----------------------------------------------------------------

bool GameRunnable::mousePressed(const OgreBites::MouseButtonEvent &evt)
{
	bool mbRight = false;

	if (evt.button == SDL_BUTTON_RIGHT) {
		mbRight = true;
	}

	Ogre::String objectName = "";
	Ogre::Ray mouseRay = mTrayMgr->getCursorRay(mCam);

	mRayScnQuery->setRay(mouseRay);
	Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
	Ogre::RaySceneQueryResult::iterator it = result.begin();

	for (; it != result.end(); it++)
	{
		objectName = it->movable->getName();
	}

	//objectName = objectName.substr(7, objectName.length());
	if (mbRight) {

		std::ostringstream oss;
		oss << objectName;
		mCordPanel->setText(oss.str());
		
		// Used for right click movement... basic click to move
		Ogre::Vector2 SquareIndex = gridIndexFinder(objectName);
		PathFinding* path = new PathFinding(SquareIndex);

		for (std::vector<Unit>::iterator unit = units.begin(); unit != units.end(); ++unit) {

			//TODO temporary for flocking work. Remove or refactor. Would like to see a single method with code the render loop uses.
			Ogre::Vector2 aPos = numericalCordFinder(path->flowField[unit->currentPos.x][unit->currentPos.y]);
			Ogre::Vector3* nextCord = new Ogre::Vector3(aPos.x, 0, aPos.y);
			unit->walkList.push_back(*nextCord);

			Ogre::Vector2 finalPosition = numericalCordFinder(SquareIndex);
			Ogre::Vector3* finalCord = new Ogre::Vector3(finalPosition.x, 0, finalPosition.y);
			unit->finalDestination = *finalCord;

			//TODO encapsulate this
			if (unit->path != NULL) {
				if (unit->path->pathingUnits <= 1) {
					delete unit->path;
				}
				else {
					unit->path->pathingUnits--;
				}
			} 	
			unit->path = path;
			unit->path->pathingUnits++;
		}
		
		//mWalkList.push_back(Ogre::Vector3(cords.x, 0, cords.y));
	}
	else {
		int squareNumber = stoi(objectName);
		//TODO: do we need x & y here?
		int x = 0;
		int z = 0;
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
		Ogre::Vector2 startpos = numericalCordFinder(Ogre::Vector2(xcount, ycount));
		if (createUnitMode) {
			Ogre::String robotName = "RobotNode" + Ogre::StringConverter::toString(robotNumber);
			robotNumber++;
			
			Unit* unit = new Unit(mScnMgr, Ogre::Vector3(startpos.x, 0.0f, startpos.y), robotName);
			units.push_back(*unit);
		}


		Ogre::Vector3 clickableSquareCords = Ogre::Vector3(startpos.x, 0.0f, startpos.y);

		std::ostringstream oss;
		oss << " for " << xcount << "," << ycount;
		mCordPanel->setText(oss.str());
	}

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
	
	Ogre::String robotName1 = "RobotNode" + Ogre::StringConverter::toString(robotNumber);
	robotNumber++;
	Ogre::String robotName2 = "RobotNode" + Ogre::StringConverter::toString(robotNumber);
	robotNumber++;
	Ogre::String robotName3 = "RobotNode" + Ogre::StringConverter::toString(robotNumber);
	robotNumber++;
	Ogre::String robotName4 = "RobotNode" + Ogre::StringConverter::toString(robotNumber);
	robotNumber++;/*
	Ogre::String robotName5 = "RobotNode" + Ogre::StringConverter::toString(robotNumber);
	robotNumber++;
	Ogre::String robotName6 = "RobotNode" + Ogre::StringConverter::toString(robotNumber);
	robotNumber++;
	Ogre::String robotName7 = "RobotNode" + Ogre::StringConverter::toString(robotNumber);
	robotNumber++;
	Ogre::String robotName8 = "RobotNode" + Ogre::StringConverter::toString(robotNumber);
	robotNumber++;*/
	Unit* unit;
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 45.0f), robotName1);
	unit->unitNode->setPosition(15.0f, 0.0f, 45.0f);
	unit->currentPos = Ogre::Vector2(0, 0);
	unit->realizedPosition = Ogre::Vector2(0, 0);
	units.push_back(*unit);
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 100.0f), robotName2);
	unit->currentPos = Ogre::Vector2(0, 1);
	units.push_back(*unit);
	unit = new Unit(mScnMgr, Ogre::Vector3(65.0f, 0.0f, 50.0f), robotName3);
	unit->currentPos = Ogre::Vector2(1, 0);
	units.push_back(*unit);
	unit = new Unit(mScnMgr, Ogre::Vector3(65.0f, 0.0f, 100.0f), robotName4);
	unit->currentPos = Ogre::Vector2(1, 1);
	units.push_back(*unit);/*
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 150.0f), robotName5);
	unit->currentPos = Ogre::Vector2(0, 0);
	units.push_back(*unit);
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 200.0f), robotName6);
	unit->currentPos = Ogre::Vector2(0, 1);
	units.push_back(*unit);
	unit = new Unit(mScnMgr, Ogre::Vector3(65.0f, 0.0f, 150.0f), robotName7);
	unit->currentPos = Ogre::Vector2(1, 0);
	units.push_back(*unit);
	unit = new Unit(mScnMgr, Ogre::Vector3(65.0f, 0.0f, 200.0f), robotName8);
	unit->currentPos = Ogre::Vector2(1, 1);
	units.push_back(*unit);*/
	unit->unitAnimState = unit->unitEntity->getAnimationState("Idle");
	unit->unitAnimState->setLoop(true);
	unit->unitAnimState->setEnabled(true);

	/*
	Ogre::String robotName1 = CONSTANTS->robotNode + Ogre::StringConverter::toString(robotNumber);
	robotNumber++;
	Unit* unit;
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 50.0f), robotName1);
	unit->currentPos = Ogre::Vector2(0, 0);
	units.push_back(*unit);
	*/
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

	//Ogre::Entity* ent = mScnMgr->getEntity("Ogre/MO60");
	Ogre::Entity* ent0 = mScnMgr->getEntity("83");
	Ogre::Entity* ent6 = mScnMgr->getEntity("82");
	Ogre::Entity* ent7 = mScnMgr->getEntity("93");
	Ogre::Entity* ent8 = mScnMgr->getEntity("94");
	Ogre::Entity* ent1 = mScnMgr->getEntity("47");
	Ogre::Entity* ent2 = mScnMgr->getEntity("48");
	Ogre::Entity* ent3 = mScnMgr->getEntity("70");
	Ogre::Entity* ent4 = mScnMgr->getEntity("59");
	Ogre::Entity* ent5 = mScnMgr->getEntity("56");
	ent0->setMaterialName("Examples/circuit");
	ent6->setMaterialName("Examples/circuit");
	ent7->setMaterialName("Examples/circuit");
	ent8->setMaterialName("Examples/circuit");
	ent1->setMaterialName("Examples/circuit");
	ent2->setMaterialName("Examples/circuit");
	ent3->setMaterialName("Examples/circuit");
	ent4->setMaterialName("Examples/circuit");
	ent5->setMaterialName("Examples/circuit");

	/* END TESTING ZONE */
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

Ogre::Vector3 GameRunnable::staticObjectCollisionForceApplier(Unit unit) {
	Ogre::Vector3 totalForce = Ogre::Vector3(0, 0, 0);
	getAllNeighbors(unit);
	int fedge = (Constants::edgeLength * 0.7) / 2;
	int neighborsCount = 0;

	while (!unitNeighbors.empty()) {
		SquareNeighbor* neighbor = unitNeighbors.front();
		if (neighbor->getDistance() == Constants::WALL) {
			Ogre::Vector2 cords = numericalCordFinder(neighbor->getPosition());
			Ogre::Vector3 terrain(cords.x, 0, cords.y);
			int a, b, c;
			a = fedge;
			b = 0;

			/* unit is above or below */
			if (neighbor->getPosition().x == unit.currentPos.x) {
				int b = std::abs(unit.getPosition().x - terrain.x);
			}
			/* unit is to the sides */
			if (neighbor->getPosition().y == unit.currentPos.y) {
				int b = std::abs(unit.getPosition().y - terrain.y);
			}
			c = std::sqrt((a * a) + (b * b));

			int distance = std::abs(unit.getPosition().length() - terrain.length());

			if (distance < c) {
				Ogre::Vector3* pushForce = subtractVector(unit.getPosition(), terrain);
				totalForce.operator+=(pushForce->operator/(unit.seperationRadius));

				neighborsCount++;
				free(pushForce);
			}
		}
		free(neighbor);
		unitNeighbors.pop();
	}

	if (neighborsCount == 0) {
		return totalForce;
	}

	totalForce.operator/=(neighborsCount);
	totalForce.operator*=(unit.walkSpeed);

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
	

	/* Robot animation and movement */
	for (std::vector<Unit>::iterator unit = units.begin(); unit != units.end(); ++unit) {
		if (unit->unitAnimState->getAnimationName() == "Walk") {
			
			
			/* Realtime directing */
			Ogre::Vector2 position = cordNumericalFinder(unit->getPosition());
			if (position != unit->currentPos) {
				unit->currentPos = position;
			}

			Ogre::Vector3 cords = Ogre::Vector3::ZERO;
			if (unit->hasLos()) {
				cords = unit->finalDestination;
			}
			else {
				Ogre::Vector2 direction = numericalCordFinder(*unit->getCurrentFlowValue());
				int x, y;
				x = direction.x;
				y = direction.y;
				cords = Ogre::Vector3(x, 0, y);
			}
			
			Ogre::Vector3 collision = staticObjectCollisionForceApplier(*unit);
			Ogre::Vector3 seek = SteeringBehaviour::seek(*unit, cords);
			Ogre::Vector3 seperation = SteeringBehaviour::seperation(*unit, &units);
			Ogre::Vector3 cohesion = SteeringBehaviour::cohesion(*unit, &units);
			Ogre::Vector3 alignment = SteeringBehaviour::alignment(*unit, &units);

			unit->forceToApply = seek.operator+=(seperation).operator+=(cohesion.operator*=(0.1)).operator+=(alignment).operator+=(collision);
			//unit->forceToApply = seek.operator+=(seperation).operator+=(cohesion.operator*=(0.1)).operator+=(alignment);
		}
	}

	Ogre::AnimationState* tempState;
	for (std::vector<Unit>::iterator unit = units.begin(); unit != units.end(); ++unit) {

		if (unit->nextLocation()) {
			unit->animate("Walk");
			unit->velocity = unit->direction;
		}
		if (unit->destination != Ogre::Vector3::ZERO) {
			//unit->velocity.operator+=(unit->forceToApply.operator*=(evt.timeSinceLastFrame));		//Delta Time
			unit->velocity.operator+=(unit->forceToApply.operator*=(0.0166666));					//Mocked Delta Time
			float speed = unit->velocity.length();
			if (speed > unit->maxSpeed) {
				unit->velocity.operator*=(unit->maxSpeed / speed);
			}

			unit->rotate(unit->velocity);

			
			Ogre::Vector3 newPos = unit->getPosition().operator+=(unit->velocity.operator*(0.0166666));
			//Ogre::Vector3 newPos = unit->getPosition().operator+=(unit->velocity.operator*(evt.timeSinceLastFrame));
			unit->unitNode->setPosition(newPos);
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