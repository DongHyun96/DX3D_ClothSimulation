#include "Framework.h"
#include "GameScene.h"


GameScene::GameScene()
{
	//model = new Model("Adeon");

	adeon = new Car("Adeon");
}

GameScene::~GameScene()
{
	//delete model;

	delete adeon;
}

void GameScene::Update()
{
	//model->Update();
	//
	//if (KEY_PRESS(VK_UP)) model->translation	+=	DELTA_TIME * model->GetForwardVector() * 10.f;
	//if (KEY_PRESS(VK_RIGHT)) model->translation +=	DELTA_TIME * model->GetRightVector() * 10.f;

	adeon->Update();
}

void GameScene::Render()
{
	//model->Render();
	adeon->Render();
}

void GameScene::PreRender()
{
}

void GameScene::PostRender()
{
	//model->Debug();
	adeon->Debug();
}
