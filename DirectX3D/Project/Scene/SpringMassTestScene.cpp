#include "Framework.h"
#include "SpringMassTestScene.h"


SpringMassTestScene::SpringMassTestScene()
{
	Init();
}

SpringMassTestScene::~SpringMassTestScene()
{
	delete floor;

	for (RigidSphere*& particle : particles)
		delete particle;

	particles.clear();

	for (Spring*& spring : springs)
		delete spring;

	springs.clear();

	delete obstacle;
}

/*
	for Step Rate times...
		Clear All forces
		Particle Add force(Gravity, own velocity)
		Spring Add force to each Particles
		Particle steps(UpdateRigidBody)
		ResolveCollisions
*/
void SpringMassTestScene::Update()
{
	floor->Update();
	obstacle->Update();
	
	for (UINT i = 0; i < 100; i++)
	{
		for (auto& particle : particles)
		{
			particle->ClearForce();
			particle->AddVelocity();
		}

		for (auto& spring : springs)
			spring->AddForceToParticles();

		for (auto& particle : particles)
		{
			particle->UpdateRigidBody(100);
			particle->Update();

			if (((ColliderSphere*)particle)->Collision(obstacle))
				particle->HandleCollision(obstacle);

			if (particle->Collision(floor))
				particle->HandleCollision(floor);
		}
	}

	for (auto& spring : springs)
		spring->Update();

	
	if (KEY_DOWN('1'))		particles[FIXED_LEFT_IDX]->SetFixed(false);
	else if (KEY_DOWN('2')) particles[FIXED_RIGHT_IDX]->SetFixed(false);

}

// Draw call - 1882
void SpringMassTestScene::Render()
{
	floor->Render();

	//for (RigidSphere*& particle : particles)
	//	particle->Render();
	
	for (Spring*& spring : springs)
		spring->Render();

	obstacle->Render();
}

void SpringMassTestScene::PreRender()
{
}

void SpringMassTestScene::PostRender()
{
	obstacle->Debug();
}

/*
	particle - 400°³
	spring	 - 1482°³
*/
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

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			RigidSphere* particle = new RigidSphere(0.01f, 0.1f);
			particle->translation = { (x - 1) * 2.f, 100.f - (y - 1) * 2.f, Random(0.f, 1.f) };
			//particle->SetFixed(true);
			particles.push_back(particle);
		}
	}

	for (int y = 0; y < 20; y++)
		for (int x = 0; x < 19; x++)
			springs.push_back(new Spring(particles[(y) * 20 + x], particles[(y) * 20 + x + 1]));
			
	for (int y = 0; y < 19; y++) 
		for (int x = 0; x < 20; x++)
			springs.push_back(new Spring(particles[(y) * 20 + x], particles[(y + 1) * 20 + x]));

	for (int y = 0; y < 19; y++)
		for (int x = 0; x < 19; x++)
			springs.push_back(new Spring(particles[(y) * 20 + x], particles[(y + 1) * 20 + x + 1], 200.f));

	for (int y = 0; y < 19; y++)
		for (int x = 0; x < 19; x++)
			springs.push_back(new Spring(particles[(y + 1) * 20 + x], particles[(y) * 20 + x + 1], 200.f));

	particles[FIXED_LEFT_IDX]->SetFixed(true);
	particles[FIXED_RIGHT_IDX]->SetFixed(true);

	obstacle = new ColliderSphere(10.f);


}
