#include "Framework.h"
#include "GameScene.h"


GameScene::GameScene()
{
	wheel = new Model("Wheel");
}


GameScene::~GameScene()
{
	delete wheel;
}

void GameScene::Update()
{
	wheel->Update();
}

void GameScene::Render()
{
	wheel->Render();
}

void GameScene::PreRender()
{
}

void GameScene::PostRender()
{
	wheel->Debug();
}
