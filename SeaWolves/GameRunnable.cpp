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
	dijkastraGrid2d(CONSTANTS->dimension, std::vector<int>(CONSTANTS->dimension, NULL)),
	flowField(CONSTANTS->dimension),
	losGrid(CONSTANTS->dimension, std::vector<bool>(CONSTANTS->dimension, false)),
	flowFieldLock(false),
	createUnitMode(false),
	currentPos(0, 0),
	robotNumber(0)
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
	for (int i = 0; i < (CONSTANTS->gridHeight * CONSTANTS->gridWidth); ++i) {
		int temp = i;
		if ((temp % CONSTANTS->gridWidth) == 0) {
			xscale = CONSTANTS->edgeLength;
			yscale += CONSTANTS->edgeLength;
		}
		else {
			xscale += CONSTANTS->edgeLength;
		}
		std::stringstream meshName;
		meshName << i;
		std::string strToPass = meshName.str();
		if (temp % 2 == 0) {
			createSquare(xscale, yscale, CONSTANTS->edgeLength, strToPass, true, (Ogre::ColourValue::Green));
		}
		else {
			createSquare(xscale, yscale, CONSTANTS->edgeLength, strToPass, false, (Ogre::ColourValue::Red));
		}
	}
}
//----------------------------------------------------------------

Ogre::Vector2 GameRunnable::gridSquareCordFinder(std::string squareName)
{
	int squareNumber = stoi(squareName);
	float fedge = CONSTANTS->edgeLength * 0.7f;
	int x = 0;
	int z = 0;
	int xcount = 0;
	int ycount = 0;
	int total = 0;
	bool end = false;
	for (xcount; xcount < CONSTANTS->dimension; xcount++) {
		total = xcount;
		while (total <= squareNumber) {
			if (total == squareNumber) {
				end = true;
				break;
			}
			else {
				total += CONSTANTS->dimension;
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
	float fedge = CONSTANTS->edgeLength * 0.7f;
	x = (cordinates.x * fedge) + (fedge * 0.5);
	y = (cordinates.y * fedge) + (fedge * 1.5);
	Ogre::Vector2 cords(x, y);
	return cords;
}
//----------------------------------------------------------------

Ogre::Vector3 GameRunnable::numericalCordFinder(Ogre::Vector3 cordinates) {
	int x = 0;
	int z = 0;
	float fedge = CONSTANTS->edgeLength * 0.7f;
	x = (cordinates.x * fedge) + (fedge * 0.5);
	z = (cordinates.z * fedge) + (fedge * 1.5);
	Ogre::Vector3 cords(x, 0, z);
	return cords;
}
//----------------------------------------------------------------

Ogre::Vector3 GameRunnable::vector2dTo3d(Ogre::Vector2 squareIndex) {
	return Ogre::Vector3(squareIndex.x, 0, squareIndex.y);
}
//----------------------------------------------------------------

Ogre::Vector2 GameRunnable::gridIndexFinder(Ogre::String squareName) {
	int squareNumber = stoi(squareName);
	
	int xcount = 0;
	int ycount = 0;
	int total = 0;
	bool end = false;
	for (xcount; xcount < CONSTANTS->dimension; xcount++) {
		total = xcount;
		while (total <= squareNumber) {
			if (total == squareNumber) {
				end = true;
				break;
			}
			else {
				total += CONSTANTS->dimension;
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

void GameRunnable::generateDijkastraGrid2(Ogre::Vector2 point) {

	//dijkastraGrid2d(11, std::vector<int>(11)) = new std::vector<std::vector<int>>(CONSTANTS->dimension, std::vector<int> (CONSTANTS->dimension, NULL));

	for (int x = 0; x < CONSTANTS->dimension; x++) {
		//dijkastraGrid2d[x] = std::vector<int>(11);

		for (int y = 0; y < CONSTANTS->dimension; y++) {
			dijkastraGrid2d[x][y] = NULL;
			losGrid[x][y] = false;
		}
	}

	//Set impassable terrain
	dijkastraGrid2d[3][2] = CONSTANTS->WALL;
	dijkastraGrid2d[3][4] = CONSTANTS->WALL;
	dijkastraGrid2d[4][5] = CONSTANTS->WALL;
	dijkastraGrid2d[1][5] = CONSTANTS->WALL;
	dijkastraGrid2d[7][7] = CONSTANTS->WALL;
	dijkastraGrid2d[6][7] = CONSTANTS->WALL;

	//Set point of origin to 0
	int x, y;
	x = point.x;
	y = point.y;
	dijkastraGrid2d[x][y] = 1;
	losGrid[x][y] = true;
	SquareNeighbor* origin = new SquareNeighbor(Ogre::Vector2(x, y), 1);

	std::queue<SquareNeighbor*> toVisit;
	toVisit.push(origin);

	while (!toVisit.empty()) {
		SquareNeighbor* start = toVisit.front();

		//Calculate if we have LOS
		//Only need to see if don't have LOS if we aren't the end
		if (start->getDistance() != origin->getDistance()) {
			calculateLos(*start, *origin);	
		}

		getNeighbors(*start);

		while (!squareNeighbors.empty()) {
			SquareNeighbor* neighbor = squareNeighbors.front();
			int x = neighbor->getPosition().x;
			int y = neighbor->getPosition().y;

			if ((x > -1 && y > -1) && (x < 11 && x < 11)) {
				if (dijkastraGrid2d[x][y] == 0) {
					neighbor->setDistance(start->getDistance() + 1);
					dijkastraGrid2d[x][y] = neighbor->getDistance();
					toVisit.push(neighbor);
				}
			}
			squareNeighbors.pop();
		}

		toVisit.pop();
	}
}

void GameRunnable::calculateLos(SquareNeighbor at, SquareNeighbor pathEnd) {
	float xDif = pathEnd.getPosition().x - at.getPosition().x;
	float yDif = pathEnd.getPosition().y - at.getPosition().y;

	float xDifAbs = std::abs(xDif);
	float yDifAbs = std::abs(yDif);

	bool hasLos = false;

	float xDifOne = (xDif > 0) ? 1 : ((xDif < 0) ? -1 : 0);
	float yDifOne = (yDif > 0) ? 1 : ((yDif < 0) ? -1 : 0);;

	//Check in the x direction
	if (xDifAbs >= yDifAbs) {

		if (losGrid[at.getPosition().x + xDifOne][at.getPosition().y]) {
			hasLos = true;
		}
	}
	//Check in the y direction
	if (yDifAbs >= xDifAbs) {

		if (losGrid[at.getPosition().x][at.getPosition().y + yDifOne]) {
			hasLos = true;
		}
	}

	//If we are not a straight line vertically/horizontally to the exit
	if (yDifAbs > 0 && xDifAbs > 0) {
		//If the diagonal doesn't have LOS, we don't
		if (!losGrid[at.getPosition().x + xDifOne][at.getPosition().y + yDifOne]) {
			hasLos = false;
		}
		else if (yDifAbs == xDifAbs) {
			//If we are an exact diagonal and either straight direction is a wall, we don't have LOS
			if (dijkastraGrid2d[at.getPosition().x + xDifOne][at.getPosition().y] == CONSTANTS->WALL || dijkastraGrid2d[at.getPosition().x][at.getPosition().y + yDifOne] == CONSTANTS->WALL) {
				hasLos = false;
			}
		}
	}
	//It's a definite now
	losGrid[at.getPosition().x][at.getPosition().y] = hasLos;
}

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

void GameRunnable::generateFlowField() {

	flowFieldLock = false;
	//Generate an empty grid, set all places as Vector2.zero, which will stand for no good direction
	for (int x = 0; x < CONSTANTS->dimension; x++) {
		std::vector<Ogre::Vector2> vec(CONSTANTS->dimension);

		for (int y = 0; y < CONSTANTS->dimension; y++) {
			Ogre::Vector2 squareCords = Ogre::Vector2::ZERO;
			vec[y] = squareCords;
		}
		flowField[x] = vec;
	}

	//For each grid square
	for (int x = 0; x < CONSTANTS->dimension; x++) {
		for (int y = 0; y < CONSTANTS->dimension; y++) {

			//obstacles have no flow value
			//TODO
			/*if (flowField[x][y] == std::numeric_limits<int>::max()) {
			continue;
			}*/

			//TODO read on resource acquisition is initilization
			//std::auto_ptr<Ogre::Vector2> pos(new Ogre::Vector2(x, y));
			//std::auto_ptr<SquareNeighbor> pos(new SquareNeighbor(Ogre::Vector2(x, y), dijkastraGrid2d[x][y]));
			SquareNeighbor pos = SquareNeighbor(Ogre::Vector2(x, y), 0);
			getAllNeighbors(pos);

			Ogre::Vector2 min = Ogre::Vector2::ZERO;
			bool minAltered = false;
			int minDistance = 0;

			while (!squareNeighbors.empty()) {
				SquareNeighbor* neighbor = squareNeighbors.front();
				int nx = neighbor->getPosition().x;
				int ny = neighbor->getPosition().y;

				if (nx < 11 && ny < 11) {
					int distance = dijkastraGrid2d[nx][ny] - dijkastraGrid2d[pos.getPosition().x][pos.getPosition().y];
					if (distance < minDistance) {
						min = Ogre::Vector2(nx, ny);
						minDistance = distance;
						minAltered = true;
					}
				}

				squareNeighbors.pop();
			}
			if (minAltered) {
				flowField[x][y] = min;
			}
		}
	}
	flowFieldLock = true;
}
//----------------------------------------------------------------

void GameRunnable::steeringBehaviourFlowField(Ogre::Vector2 pos) {

	Ogre::Vector2 f00 = flowField[pos.x][pos.y];
	Ogre::Vector2 f01 = flowField[pos.x][pos.y + 1];
	Ogre::Vector2 f10 = flowField[pos.x + 1][pos.y];
	Ogre::Vector2 f11 = flowField[pos.x + 1][pos.y + 1];

	// X interpolation
	Ogre::Vector2 floor = flowField[pos.x - 1][pos.y - 1];

	int xweight = pos.x - floor.x;

	int top = (f00.x * (1 - xweight)) + (f10.x * xweight);
	int bottom = (f01.x * (1 - xweight)) + (f11.x * xweight);

	int yweight = pos.y - floor.y;

	int direction = (top * (1 - yweight)) + (bottom);
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
		for (std::vector<Unit>::iterator unit = units.begin(); unit != units.end(); ++unit) {
			unit->finalDestination = SquareIndex;
		}
		generateDijkastraGrid2(SquareIndex);
		generateFlowField();
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
		for (xcount; xcount < CONSTANTS->dimension; xcount++) {
			total = xcount;
			while (total <= squareNumber) {
				if (total == squareNumber) {
					end = true;
					break;
				}
				else {
					total += CONSTANTS->dimension;
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

		if (createUnitMode) {
			Ogre::String robotName = CONSTANTS->robotNode + Ogre::StringConverter::toString(robotNumber);
			robotNumber++;
			Ogre::Vector2 startpos = numericalCordFinder(Ogre::Vector2(xcount, ycount));
			Unit* unit = new Unit(mScnMgr, Ogre::Vector3(startpos.x, 0.0f, startpos.y), robotName);
			units.push_back(*unit);
		}

		int distance = dijkastraGrid2d[xcount][ycount];
		Ogre::Vector2 toFlow = flowField[xcount][ycount];

		std::ostringstream oss;
		/*oss << "distance: " << distance << " for " << xcount << "," << ycount << " and pathTo: " << toFlow.x << 
			"," << toFlow.y << " and has LOS: " << losGrid[xcount][ycount];*/
		bool checkval = losGrid[xcount][ycount];
		oss << xcount << "," << ycount << " has LOS: " << checkval;
		mCordPanel->setText(oss.str());
	}

	return true;
}
//----------------------------------------------------------------

void GameRunnable::findPath(Ogre::Vector2 origin, Unit unit) {
	currentPos = flowField[origin.x][origin.y];
	Ogre::Vector2 nextPos = numericalCordFinder(currentPos);
	unit.walkList.push_back(Ogre::Vector3(nextPos.x, 0, nextPos.y));
}
//----------------------------------------------------------------

void GameRunnable::setup(void)
{
	// do not forget to call the base first
	OgreBites::ApplicationContext::setup();
	addInputListener(this);
	SDL_ShowCursor(SDL_DISABLE);
	CONSTANTS = new Constants();
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

	Ogre::String robotName1 = CONSTANTS->robotNode + Ogre::StringConverter::toString(robotNumber);
	robotNumber++;
	Ogre::String robotName2 = CONSTANTS->robotNode + Ogre::StringConverter::toString(robotNumber);
	robotNumber++;
	Ogre::String robotName3 = CONSTANTS->robotNode + Ogre::StringConverter::toString(robotNumber);
	robotNumber++;
	Ogre::String robotName4 = CONSTANTS->robotNode + Ogre::StringConverter::toString(robotNumber);
	robotNumber++;
	Unit* unit;
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 50.0f), robotName1);
	units.push_back(*unit);
	unit = new Unit(mScnMgr, Ogre::Vector3(15.0f, 0.0f, 400.0f), robotName2);
	units.push_back(*unit);
	unit = new Unit(mScnMgr, Ogre::Vector3(365.0f, 0.0f, 50.0f), robotName3);
	units.push_back(*unit);
	unit = new Unit(mScnMgr, Ogre::Vector3(365.0f, 0.0f, 400.0f), robotName4);
	units.push_back(*unit);

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
	Ogre::Entity* ent1 = mScnMgr->getEntity("84");
	Ogre::Entity* ent2 = mScnMgr->getEntity("25");
	Ogre::Entity* ent3 = mScnMgr->getEntity("47");
	Ogre::Entity* ent4 = mScnMgr->getEntity("59");
	Ogre::Entity* ent5 = mScnMgr->getEntity("56");
	ent0->setMaterialName("Examples/circuit");
	ent1->setMaterialName("Examples/circuit");
	ent2->setMaterialName("Examples/circuit");
	ent3->setMaterialName("Examples/circuit");
	ent4->setMaterialName("Examples/circuit");
	ent5->setMaterialName("Examples/circuit");

	/* END TESTING ZONE */
}
//----------------------------------------------------------------


void GameRunnable::frameRendered(const Ogre::FrameEvent& evt)
{
	/* Camera movement */
	/* Working code but annoying when trying to debug
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
	*/

	/* Robot animation and movement */
	Ogre::AnimationState* tempState;
	for (std::vector<Unit>::iterator unit = units.begin(); unit != units.end(); ++unit) {

		if (unit->unitAnimState != NULL) {
			if (unit->direction == Ogre::Vector3::ZERO) {
				if (unit->nextLocation()) {

					Ogre::Vector3 src = unit->unitNode->getOrientation() * Ogre::Vector3::UNIT_X;

					/* Unit rotation code */
					if ((1.0 + src.dotProduct(unit->direction)) < 0.0001) {
						unit->unitNode->yaw(Ogre::Degree(180));
					}
					else {
						Ogre::Quaternion quat = src.getRotationTo(unit->direction);
						unit->unitNode->rotate(quat);
					}
					// TODO: replace with units vector
					unit->animate("Walk");
					
				}
				else if (flowFieldLock && flowField[unit->currentPos.x][unit->currentPos.y] != Ogre::Vector2::ZERO) {
					Ogre::Vector3* nextCord;
					if (losGrid[unit->currentPos.x][unit->currentPos.y]) {
						Ogre::Vector2 finalCords = numericalCordFinder(unit->finalDestination);
						unit->currentPos = unit->finalDestination;
						nextCord = new Ogre::Vector3(finalCords.x, 0, finalCords.y);
					}
					else {
						Ogre::Vector2 currPos = numericalCordFinder(unit->currentPos);
						unit->currentPos = flowField[unit->currentPos.x][unit->currentPos.y];
						Ogre::Vector2 nextPos = numericalCordFinder(unit->currentPos);

						if (dijkastraGrid2d[unit->currentPos.x][unit->currentPos.y] != 1) {
							nextCord = interpolateMovement(currPos, nextPos);
						}
						else {
							nextCord = new Ogre::Vector3(nextPos.x, 0, nextPos.y);
						}
					}

					unit->walkList.push_back(*nextCord);	
				}
			}
			else {
				//Ogre::Real move = mWalkSpeed * evt.timeSinceLastFrame;
				//mDistance -= move;

				Ogre::Real move = unit->walkSpeed * evt.timeSinceLastFrame;
				unit->distance -= move;
				// New code related to flowField movement
				/*if (mDistance <= 0.0f && robotEntity->getParentNode()->getPosition()) {

				}*/
				if (unit->distance <= 0.0f) {

					unit->unitNode->setPosition(unit->destination);
					unit->direction = Ogre::Vector3::ZERO;

					// Consider adding rotation code (intermediate tutorial1)
					unit->animate("Idle");
				}
				else {
					//mUnitNode->translate(move * mDirection);
					
					unit->unitNode->translate(move * unit->direction);
				}
			}

			unit->unitAnimState->addTime(evt.timeSinceLastEvent);
			//robotAnimState->addTime(evt.timeSinceLastFrame);
		}
	}
	mTrayMgr->frameRendered(evt);
}
//----------------------------------------------------------------


Ogre::Vector3* GameRunnable::interpolateMovement(Ogre::Vector2 start, Ogre::Vector2 end) {
	int x, y;
	x = (start.x + end.x) / 2;
	y = (start.y + end.y) / 2;
	return new Ogre::Vector3(x, 0, y);
}
//----------------------------------------------------------------

bool GameRunnable::nextLocation()
{
	if (mWalkList.empty()) { return false; }
	mDestination = mWalkList.front();  // this gets the front of the deque
	mWalkList.pop_front();             // this removes the front of the deque
	mDirection = mDestination - mUnitNode->getPosition();
	mDistance = mDirection.normalise();
	return true;
}
//----------------------------------------------------------------

void GameRunnable::getNeighbors(SquareNeighbor squareNeighbor) {
	int num = 0;
	int x, y;
	x = squareNeighbor.getPosition().x;
	y = squareNeighbor.getPosition().y;
	bool topEdge = false;
	bool bottomEdge = false;
	bool leftEdge = false;
	bool rightEdge = false;
	std::queue<Ogre::Vector2> neighbors;
	if ((x - 1) < 0) {
		leftEdge = true;
	}
	else if ((x + 1) > CONSTANTS->dimension) {
		rightEdge = true;
	}

	if ((y - 1) < 0) {
		topEdge = true;
	}
	else if ((y + 1) > CONSTANTS->dimension) {
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

		SquareNeighbor* squareNeighbor = new SquareNeighbor(neighborPoint, 0);
		//squareNeighbor->setDistance(dijkastraGrid2d[x][y]);

		squareNeighbors.push(squareNeighbor);
		neighbors.pop();
	}
}


void GameRunnable::getAllNeighbors(SquareNeighbor squareNeighbor) {
	int num = 0;
	int x, y;
	x = squareNeighbor.getPosition().x;
	y = squareNeighbor.getPosition().y;
	bool topEdge = false;
	bool bottomEdge = false;
	bool leftEdge = false;
	bool rightEdge = false;
	std::queue<Ogre::Vector2> neighbors;
	if ((x - 1) < 0) {
		leftEdge = true;
	}
	else if ((x + 1) > CONSTANTS->dimension) {
		rightEdge = true;
	}

	if ((y - 1) < 0) {
		topEdge = true;
	}
	else if ((y + 1) > CONSTANTS->dimension) {
		bottomEdge = true;
	}
	if (topEdge && leftEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (topEdge && rightEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x - 1, y + 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
	}
	else if (bottomEdge && leftEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (bottomEdge && rightEdge) {
		//neighbors = new Ogre::Vector2[3];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
	}
	else if (topEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x - 1, y + 1));
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (bottomEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x - 1, y - 1));
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
	}
	else if (leftEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y - 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
		neighbors.push(Ogre::Vector2(x + 1, y + 1));
		neighbors.push(Ogre::Vector2(x, y + 1));
	}
	else if (rightEdge) {
		//neighbors = new Ogre::Vector2[5];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x - 1, y + 1));
		neighbors.push(Ogre::Vector2(x, y + 1));
	}
	else {
		//all neighbors are valid
		//neighbors = new Ogre::Vector2[9];
		neighbors.push(Ogre::Vector2(x, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y - 1));
		neighbors.push(Ogre::Vector2(x - 1, y));
		neighbors.push(Ogre::Vector2(x - 1, y + 1));
		neighbors.push(Ogre::Vector2(x, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y + 1));
		neighbors.push(Ogre::Vector2(x + 1, y));
		neighbors.push(Ogre::Vector2(x + 1, y - 1));
	}

	while (!neighbors.empty()) {
		Ogre::Vector2 neighbor = neighbors.front();
		int x, y;
		x = neighbor.x;
		y = neighbor.y;
		Ogre::Vector2 neighborPoint = Ogre::Vector2(x, y);

		SquareNeighbor* squareNeighbor = new SquareNeighbor(neighborPoint, 0);
		//squareNeighbor->setDistance(dijkastraGrid2d[x][y]);

		squareNeighbors.push(squareNeighbor);
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