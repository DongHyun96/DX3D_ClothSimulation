#include "Framework.h"
#include "RigidBodyTestScene.h"


RigidBodyTestScene::RigidBodyTestScene()
{
	floor = new Quad();

	floor->scale *= 500.f;
	floor->rotation.x = XM_PIDIV2;
	floor->translation.y = 0.1f;
	floor->GetMaterial()->SetShader(L"16_Light");
	floor->GetMaterial()->SetDiffuseMap(L"Landscape/Dirt3.png");

	//rigidSphere = new RigidSphere(0.005f, 0.5f);
	//rigidSphere->translation.y = 100.f;

	for (UINT i = 0; i < 30; i++)
	{
		RigidSphere* rigidSphere = new RigidSphere(0.003f);
		rigidSphere->SetFixed(true);

		rigidSphere->translation.y = 100.f;

		rigidSphere->translation.x = Random(-3.f, 3.f);
		rigidSphere->translation.z = Random(-3.f, 3.f);
		rigidSpheres.push_back(rigidSphere);
	}


	obstacle = new ColliderSphere(10.f);
	//obstacle->translation.x = 2.f;
}

RigidBodyTestScene::~RigidBodyTestScene()
{
	delete floor;
	//delete rigidSphere;

	for (RigidSphere* rigidSphere : rigidSpheres)
		delete rigidSphere;

	delete obstacle;
}

void RigidBodyTestScene::Update()
{
	floor->Update();
	obstacle->Update();

	if (KEY_DOWN(VK_LBUTTON))
	{
		for (RigidSphere* rigidSphere : rigidSpheres)
			rigidSphere->SetFixed(false);
	}
	if (KEY_DOWN(VK_UP))
	{
		for (RigidSphere* rigidSphere : rigidSpheres)
		{
			rigidSphere->SetFixed(true);

			rigidSphere->translation.y = 100.f;

			rigidSphere->translation.x = Random(-3.f, 3.f);
			rigidSphere->translation.z = Random(-3.f, 3.f);
		}
	}

	for (RigidSphere* rigidSphere : rigidSpheres)
	{
		rigidSphere->Update();

		if (rigidSphere->Collision(floor))
			rigidSphere->HandleCollision(floor);

		if (((ColliderSphere*)rigidSphere)->Collision(obstacle))
			rigidSphere->HandleCollision(obstacle);
	}

}

void RigidBodyTestScene::Render()
{
	floor->Render();

	for (RigidSphere* rigidSphere : rigidSpheres)
		rigidSphere->Render();

	obstacle->Render();
}

void RigidBodyTestScene::PreRender()
{
}

void RigidBodyTestScene::PostRender()
{
}
