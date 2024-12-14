#include "Framework.h"
#include "RigidBodyTestScene.h"


RigidBodyTestScene::RigidBodyTestScene()
{
	static int index = 0;

	Quad* floor{};

	for (UINT i = 0; i < 3; i++)
	{
		floor = new Quad();

		floor->scale *= 250.f;
		floor->rotation.x = XM_PIDIV2;
		floor->translation.y = 0.1f;
		floor->GetMaterial()->SetShader(L"16_Light");
		floor->GetMaterial()->SetDiffuseMap(L"Landscape/Dirt3.png");

		floor->SetName("RigidTestFloor_" + to_string(index++));
		floor->LoadTransform();

		floors.push_back(floor);
	}

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
	//delete floor;
	//delete rigidSphere;

	for (Quad* floor : floors)
		delete floor;

	for (RigidSphere* rigidSphere : rigidSpheres)
		delete rigidSphere;

	delete obstacle;
}

/*
	Clear All forces
	Particle Add force(Gravity, own velocity)
	Spring Add force to each Particles
	Particle steps(UpdateRigidBody)
	ResolveCollisions
*/
void RigidBodyTestScene::Update()
{
	//floor->Update();
	for (Quad* floor : floors)
		floor->Update();

	obstacle->Update();

	HandleInput();

	for (RigidSphere* rigidSphere : rigidSpheres)
	{
		rigidSphere->Update();

		rigidSphere->ClearForce();

		rigidSphere->AddGravityForce();
		rigidSphere->AddViscousDragForce();

		rigidSphere->SolveCurrentPosition();

		//if (rigidSphere->Collision(floor))
		//	rigidSphere->HandleCollision(floor);

		if (((ColliderSphere*)rigidSphere)->Collision(obstacle))
			rigidSphere->ResolveCollision(obstacle);

		for (Quad* floor : floors)
		{
			if (rigidSphere->Collision(floor))
				rigidSphere->ResolveCollision(floor);
		}
	}

}

void RigidBodyTestScene::Render()
{
	//floor->Render();
	for (Quad* floor : floors)
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
	for (Quad* floor : floors)
		floor->Debug();
}

void RigidBodyTestScene::HandleInput()
{
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
}
