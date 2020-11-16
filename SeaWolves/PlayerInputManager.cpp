#include "PlayerInputManager.h"


PlayerInputManager::PlayerInputManager(
	Ogre::Camera* mCam,
	Ogre::PlaneBoundedVolumeListSceneQuery* mVolQuery,
	Ogre::SceneManager* mScnMgr)
{
	camera = mCam;
	volumeQuery = mVolQuery;
	sceneManager = mScnMgr;
}

PlayerInputManager::~PlayerInputManager()
{
}

void swap(float& x, float& y)
{
	float temp = x;
	x = y;
	y = temp;
}
//----------------------------------------------------------------

void gameInputActionSelect_Press() {

}
//----------------------------------------------------------------

void gameInputActionExecute_Press() {

}
//----------------------------------------------------------------

void PlayerInputManager::gameInputActionSelect_Release(SelectionBox* selectBox, Ogre::RaySceneQuery* raySceneQuery, Ogre::Ray &mouseRay) {

	if (selectBox->mStop == selectBox->mStart) {
		Ogre::String objectName = "";

		raySceneQuery->setRay(mouseRay);
		Ogre::RaySceneQueryResult& result = raySceneQuery->execute();
		Ogre::RaySceneQueryResult::iterator it = result.begin();

		
	}
}
//----------------------------------------------------------------

void gameInputActionExecute_Release() {

}
//----------------------------------------------------------------

void PlayerInputManager::performSelection(
	const Ogre::Vector2& first,
	const Ogre::Vector2& second,
	Player* activePlayer,
	std::vector<std::vector<GridSquare*>> &gridMap)
{
	float left = first.x, right = second.x;
	float top = first.y, bottom = second.y;

	if (left > right)
		swap(left, right);

	if (top > bottom)
		swap(top, bottom);

	if ((right - left) * (bottom - top) < 0.0001)
		return;

	Ogre::Ray topLeft = camera->getCameraToViewportRay(left, top);
	Ogre::Ray topRight = camera->getCameraToViewportRay(right, top);
	Ogre::Ray bottomLeft = camera->getCameraToViewportRay(left, bottom);
	Ogre::Ray bottomRight = camera->getCameraToViewportRay(right, bottom);

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

	volumeQuery->setVolumes(volList);
	Ogre::SceneQueryResult &result = volumeQuery->execute();

	clearFocusedLocations(activePlayer, gridMap);
	activePlayer->focusUnits(result);

}
//----------------------------------------------------------------

//TODO maybe this should live somewhere else
void PlayerInputManager::clearFocusedLocations(
	Player* activePlayer,
	std::vector<std::vector<GridSquare*>> &gridMap) {

	if (activePlayer->unitQueue.size() > 0) {
		PathFinding* path = activePlayer->unitQueue.front()->path;
		if (path != NULL) {
			//TODO this is broken now that we've switched to using textures
			/*for (std::vector<Ogre::Vector2*>::iterator it = path->formationLocations.begin(); it != path->formationLocations.end(); ++it) {
				gridMap[(*it)->x][(*it)->y]->defaultColor(sceneManager);
			}*/
			path->formationLocations.clear();
		}
	}
}
//----------------------------------------------------------------
