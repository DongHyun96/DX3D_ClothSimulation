#include "Framework.h"
#include "LightScene.h"


LightScene::LightScene()
{
	floor = new Quad;
	floor->SetName("Floor");
	floor->GetMaterial()->SetShader(L"16_Light");
	//floor->GetMaterial()->SetDiffuseMap(L)

	floor->scale *= 30.f;
	floor->rotation.x = XM_PIDIV2;
	floor->translation.y = 0.1f;

	knight = new ModelAnimator("Knight");
	knight->ReadClip("Dancing Twerk");
	knight->SetShader(L"16_Light");

	sphere = new Sphere;
	sphere->SetName("Sphere");
	sphere->GetMaterial()->SetShader(L"16_Light");
	sphere->translation = { 5.f, 5.f, 0.f };

	sphere->GetMaterial()->SetDiffuseMap(L"Landscape/FieldStone_DM.tga");
	sphere->GetMaterial()->SetNormalMap(L"Landscape/FieldStone_NM.tga");
	sphere->GetMaterial()->SetSpecularMap(L"Landscape/FieldStone_SM.tga");

	depthStencil = new DepthStencil();
	renderTarget = new RenderTarget();

	Texture* texture = Texture::Add(L"Scene", renderTarget->GetSRV());
}

LightScene::~LightScene()
{
	delete floor;
	delete knight;
	delete sphere;

	delete depthStencil;
	delete renderTarget;
}

void LightScene::Update()
{
	floor->Update();
	knight->Update();
	sphere->Update();
}

void LightScene::Render()
{
	floor->Render();
	knight->Render();
	sphere->Render();
}

void LightScene::PreRender()
{
}

void LightScene::PostRender()
{
	floor->Debug();
	knight->Debug();
	sphere->Debug();
}
