#include "Framework.h"
#include "TerrainLODScene.h"


TerrainLODScene::TerrainLODScene()
{
	terrain = new TerrainLOD(L"HeightMap/HeightMap256.png");
}

TerrainLODScene::~TerrainLODScene()
{
	delete terrain;
}

void TerrainLODScene::Update()
{
	terrain->Update();
}

void TerrainLODScene::Render()
{
	terrain->Render();
}

void TerrainLODScene::PreRender()
{
}

void TerrainLODScene::PostRender()
{
	terrain->Debug();
}
