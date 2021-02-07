#include "MapManager.h"



MapManager::MapManager(Ogre::SceneManager* sceneManager, Ogre::Light* light, std::vector<GridSquare*>* impassableTerrain, b2World* world, GridEditor* gridEditor)
{
	mScnMgr = sceneManager;
	mImpassableTerrain = impassableTerrain;
	mWorld = world;
	mGridEditor = gridEditor;

	mTerrainGlobals = new Ogre::TerrainGlobalOptions();

	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(
		mScnMgr,
		Ogre::Terrain::ALIGN_X_Z,
		513, Constants::worldSize);
	mTerrainGroup->setFilenameConvention(Ogre::String("terrain"), Ogre::String("dat"));
	mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
	mTerrainGroup->setOrigin(Ogre::Vector3(1000, 0, 1000));

	configureTerrainDefaults(light);

	/*for (long x = 0; x <= 0; ++x)
		for (long y = 0; y <= 0; ++y)*/
	defineTerrain(0, 0);

	// sync load since we want everything in place when we start
	mTerrainGroup->loadAllTerrains(true);

	if (mTerrainsImported)
	{
		Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
		while (ti.hasMoreElements())
		{
			Ogre::Terrain* t = ti.getNext()->instance;
			initBlendMaps(t);
		}
	}

	mTerrainGroup->freeTemporaryResources();

	std::ifstream infile("terrain_bogaloo.txt");
	while (infile.good())
	{
		int x, y, unpassable;
		infile >> x >> y >> unpassable;
		if (unpassable != 0)
		{
			mGridEditor->addTerrainValue(x, y, mWorld, mImpassableTerrain);
		}
	}
}
//----------------------------------------------------------------

MapManager::~MapManager()
{
}
//----------------------------------------------------------------

void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
	img.load("white.bmp", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	//img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	if (flipX)
		img.flipAroundY();
	if (flipY)
		img.flipAroundX();

}
//----------------------------------------------------------------

void MapManager::defineTerrain(long x, long y)
{
	//mTerrainGroup->defineTerrain(x, y);
	Ogre::String filename = mTerrainGroup->generateFilename(x, y);
	//Ogre::String filename = "seawolves_00000000.dat";
	//Ogre::String filename = "terrain_bogaloo.dat";
	if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
	{
		mTerrainGroup->defineTerrain(x, y);
	}
	else
	{
		Ogre::Image img;
		getTerrainImage(x % 2 != 0, y % 2 != 0, img);
		mTerrainGroup->defineTerrain(x, y, &img);
		mTerrainsImported = true;
	}
}
//----------------------------------------------------------------

void MapManager::initBlendMaps(Ogre::Terrain* terrain)
{
	float minHeight0 = 20;
	float fadeDist0 = 15;
	float minHeight1 = 70;
	float fadeDist1 = 15;

	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);

	float* pBlend0 = blendMap0->getBlendPointer();
	float* pBlend1 = blendMap1->getBlendPointer();

	for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
	{
		for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
		{
			Ogre::Real tx, ty;

			blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
			*pBlend0++ = 0.1f;
			*pBlend1++ = 0.1f;
		}
	}

	blendMap0->dirty();
	blendMap1->dirty();
	blendMap0->update();
	blendMap1->update();
}
//----------------------------------------------------------------

void MapManager::configureTerrainDefaults(Ogre::Light* light)
{
	mTerrainGlobals->setMaxPixelError(8);
	mTerrainGlobals->setCompositeMapDistance(6000);

	mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(mScnMgr->getAmbientLight());
	mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

	Ogre::Terrain::ImportData& importData = mTerrainGroup->getDefaultImportSettings();
	importData.terrainSize = 513;
	importData.worldSize = Constants::worldSize;
	importData.inputScale = 200;
	importData.minBatchSize = 33;
	importData.maxBatchSize = 65;

	importData.layerList.resize(3);
	importData.layerList[0].worldSize = 500;
	importData.layerList[0].textureNames.push_back(
		"grass_green-01_diffusespecular.dds");
	importData.layerList[0].textureNames.push_back(
		"grass_green-01_normalheight.dds");
	importData.layerList[1].worldSize = 1000;
	importData.layerList[1].textureNames.push_back(
		"dirt_grayrocky_diffusespecular.dds");
	importData.layerList[1].textureNames.push_back(
		"dirt_grayrocky_normalheight.dds");
	importData.layerList[2].worldSize = 1000;
	importData.layerList[2].textureNames.push_back(
		"sand_1_diffusespecular.dds");
	importData.layerList[2].textureNames.push_back(
		"sand_1_normalheight.dds");
}
//----------------------------------------------------------------

Ogre::TerrainGroup::RayResult MapManager::getRayResult(Ogre::Ray* ray)
{
	return mTerrainGroup->rayIntersects(*ray);
}
//----------------------------------------------------------------