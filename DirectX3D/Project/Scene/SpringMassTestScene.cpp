#include "Framework.h"
#include "SpringMassTestScene.h"


SpringMassTestScene::SpringMassTestScene()
{
	Init();

}

SpringMassTestScene::~SpringMassTestScene()
{
	delete floor;
}

void SpringMassTestScene::Update()
{
}

void SpringMassTestScene::Render()
{
}

void SpringMassTestScene::PreRender()
{
}

void SpringMassTestScene::PostRender()
{
}

void SpringMassTestScene::Init()
{
	floor = new Quad();

	floor->scale *= 250.f;
	floor->rotation.x = XM_PIDIV2;
	floor->translation.y = 0.1f;
	floor->GetMaterial()->SetShader(L"16_Light");
	floor->GetMaterial()->SetDiffuseMap(L"Landscape/Dirt3.png");

	floor->SetName("RigidTestFloor_0");
	floor->LoadTransform();


}
