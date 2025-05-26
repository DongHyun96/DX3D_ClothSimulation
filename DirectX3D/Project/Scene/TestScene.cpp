#include "Framework.h"
#include "TestScene.h"


TestScene::TestScene()
{

	defaultFloor = new TextureCube;

	defaultFloor->SetName("Default floor");
	defaultFloor->SetShader(L"16_Light");
	defaultFloor->SetDiffuseMap(L"Default/TemplateGrid_albedo.png");
	defaultFloor->SetNormalMap(L"Default/TemplateGrid_normal.png");
	//defaultFloor->SetDiffuseMap(L"Landscape/Floor.png");
	// defaultFloor->SetNormalMap(L"Landscape/Floor_normal.png");
}

TestScene::~TestScene()
{
	delete defaultFloor;
}

void TestScene::Update()
{
	defaultFloor->Update();
}

void TestScene::Render()
{
	defaultFloor->Render();
}

void TestScene::PreRender()
{
}

void TestScene::PostRender()
{
	defaultFloor->Debug();
}
