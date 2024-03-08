#include "Framework.h"
#include "WaterScene.h"


WaterScene::WaterScene()
{
	CreateObjects();

	//reflection = new Reflection(floor);
	//refraction = new Refraction(L"Landscape/Wave.dds");

	//floor->GetMaterial()->SetShader(L"26_Reflection");
	//floor->GetMaterial()->SetShader(L"27_Refraction");

	sky = new SkyBox(L"Landscape/ddsTest.dds");

	water = new Water(L"Landscape/Wave.dds");
}

WaterScene::~WaterScene()
{
	delete floor;
	delete knight;
	delete sphere;

	delete bunny;

	//delete reflection;
	//delete refraction;

	delete sky;
	delete water;
}

void WaterScene::Update()
{
	floor->Update();
	knight->Update();
	sphere->Update();
	bunny->Update();

	//reflection->Update();
	//refraction->Update();
	water->Update();

	sky->Update();
}

void WaterScene::Render()
{
	sky->Render();

	//reflection->SetRender();
	//refraction->SetRender();

	water->Render();

	//floor->Render();
	knight->Render();
	sphere->Render();
	bunny->Render();
}

void WaterScene::PreRender()
{
	//reflection->SetPreRender();
	//refraction->SetPreRender();
	water->SetReflection();

	sky->Render();
	knight->Render();
	sphere->Render();
	bunny->Render();

	water->SetRefraction();

	sky->Render();
	knight->Render();
	sphere->Render();
	bunny->Render();
}

void WaterScene::PostRender()
{
	//reflection->PostRender();
	//refraction->PostRender();
	//refraction->Debug();
	water->Debug();
}

void WaterScene::CreateObjects()
{
	floor = new Quad;
	floor->SetName("Floor");
	//floor->GetMaterial()->SetDiffuseMap(L)

	floor->scale *= 30.f;
	floor->rotation.x = XM_PIDIV2;
	floor->translation.y = 0.1f;

	knight = new ModelAnimator("Knight");
	knight->ReadClip("Dancing Twerk");

	sphere = new Sphere;
	sphere->SetName("Sphere");
	sphere->translation = { 5.f, 5.f, 0.f };

	sphere->GetMaterial()->SetDiffuseMap(L"Landscape/FieldStone_DM.tga");
	sphere->GetMaterial()->SetNormalMap(L"Landscape/FieldStone_NM.tga");
	sphere->GetMaterial()->SetSpecularMap(L"Landscape/FieldStone_SM.tga");

	bunny = new Model("StanfordBunny");

	floor->GetMaterial()->SetShader(L"16_Light");
	knight->SetShader(L"16_Light");
	sphere->GetMaterial()->SetShader(L"16_Light");
	bunny->SetShader(L"16_Light");
}
