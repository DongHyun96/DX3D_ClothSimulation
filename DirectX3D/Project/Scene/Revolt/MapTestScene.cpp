#include "Framework.h"
#include "MapTestScene.h"


MapTestScene::MapTestScene()
{
	map = new Model("market2");
}

MapTestScene::~MapTestScene()
{
	delete map;
}

void MapTestScene::Update()
{
	map->Update();
}

void MapTestScene::Render()
{
	map->Render();
}

void MapTestScene::PreRender()
{
}

void MapTestScene::PostRender()
{
	map->Debug();
}
