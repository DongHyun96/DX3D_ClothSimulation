#include "Framework.h"
#include "SkyScene.h"


SkyScene::SkyScene()
{
	sky = new SkyBox(L"Landscape/ColdSunset.dds");
}

SkyScene::~SkyScene()
{
	delete sky;
}

void SkyScene::Update()
{
	sky->Update();

}

void SkyScene::Render()
{
	sky->Render();

}

void SkyScene::PreRender()
{
}

void SkyScene::PostRender()
{
}
