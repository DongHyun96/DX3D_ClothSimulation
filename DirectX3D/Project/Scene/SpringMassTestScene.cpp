#include "Framework.h"
#include "SpringMassTestScene.h"


SpringMassTestScene::SpringMassTestScene()
{
	Init();
}

SpringMassTestScene::~SpringMassTestScene()
{
	delete floor;

	delete cloth;

	//delete obstacle;
	for (ColliderSphere*& obstacle : obstacles)
		delete obstacle;
}

void SpringMassTestScene::Update()
{
	floor->Update();

	for (ColliderSphere*& obstacle : obstacles)
		obstacle->Update();

	cloth->Update();
	
}

// Draw call - 1882
void SpringMassTestScene::Render()
{
	floor->Render();

	cloth->Render();

	for (ColliderSphere*& obstacle : obstacles)
		obstacle->Render();
}

void SpringMassTestScene::PreRender()
{
}

void SpringMassTestScene::PostRender()
{
	//obstacle->Debug();
	for (ColliderSphere*& obstacle : obstacles)
		obstacle->Debug();

	floor->GetMaterial()->Debug();
}

void SpringMassTestScene::Init()
{
	floor = new Quad();

	floor->scale *= 250.f;
	floor->rotation.x = XM_PIDIV2;
	floor->translation.y = 0.1f;

	floor->GetMaterial()->SetShader(L"16_Light");
	floor->GetMaterial()->SetDiffuseMap(L"Solid/White.png");
	floor->GetMaterial()->GetBuffer()->data.diffuse = Vector4(0.286, 0.411, 0.537, 1);

	floor->SetName("RigidTestFloor_0");
	floor->LoadTransform();
	
	for (UINT i = 0; i < 3; i++)
	{
		ColliderSphere* o = new ColliderSphere(10.f);
		o->SetName("SpringMassObstacle_" + to_string(i));
		o->LoadTransform();
		obstacles.push_back(o);
	}

	cloth = new Cloth;

	cloth->AddObstacles(floor);
	
	for (ColliderSphere* cSphere : obstacles)
		cloth->AddObstacles(cSphere);


}
