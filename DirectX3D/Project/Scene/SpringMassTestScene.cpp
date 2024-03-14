#include "Framework.h"
#include "SpringMassTestScene.h"


SpringMassTestScene::SpringMassTestScene()
{
	Init();

	p1 = new RigidSphere(0.003f);
	p1->SetFixed(true);
	p1->translation = {};

	p2 = new RigidSphere(0.003f);
	p2->SetFixed(true);
	p2->translation = {0, 3, 0};

	spring = new Spring(p1, p2);
}

SpringMassTestScene::~SpringMassTestScene()
{
	delete floor;

	delete p1;
	delete p2;

	delete spring;
}

void SpringMassTestScene::Update()
{
	floor->Update();

	p1->Update();
	p2->Update();
	spring->Update();
}

void SpringMassTestScene::Render()
{
	floor->Render();

	p1->Render();
	p2->Render();
	spring->Render();
}

void SpringMassTestScene::PreRender()
{
}

void SpringMassTestScene::PostRender()
{
	//p1->Debug();
	//p2->Debug();
	//spring->Debug();
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
