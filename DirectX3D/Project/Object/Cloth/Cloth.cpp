#include "Framework.h"
#include "Cloth.h"


Cloth::Cloth(Vector4 color)
	:color(color)
{
	InitSpringsAndParticles();

	InitInstancing();

	this->Update();
	isPlaying = false;

}

Cloth::~Cloth()
{
	for (Spring*& spring : springs)
		delete spring;

	springs.clear();

	for (RigidSphere* particle : particles)
		delete particle;

	particles.clear();

	delete instanceBuffer;
	delete springBase;
}

/*
	for Step Rate times...
		Clear All forces
		Particle Add force(Gravity, own velocity)
		Spring Add force to each Particles
		Particle steps(UpdateRigidBody)
		ResolveCollisions
*/
void Cloth::Update()
{
	HandleInput();

	if (!isPlaying) return;

	for (UINT i = 0; i < 100; i++)
	{
		if (isWindActive)
		{
			windVelocity.z += ACCEL_AMOUNT * accelSign * DELTA_TIME * 0.01f;

			accelSignTimer += DELTA_TIME * 0.01f;

			if (accelSignTimer >= 1.f)
			{
				accelSignTimer -= 1.f;
				accelSign *= -1;
			}
		}

		for (auto& particle : particles)
		{
			particle->ClearForce();
			particle->AddVelocity();

			if (isWindActive)
			{
				bias = Random(-5.f, 10.f);
				particle->AddForce(windVelocity * bias);
			}
		}

		for (auto& spring : springs)
			spring->AddForceToParticles();

		for (auto& particle : particles)
		{
			particle->UpdateRigidBody(100);
			particle->Update();

			for (Quad*& quad : quadObstacles)
			{
				if (particle->Collision(quad))
					particle->ResolveCollision(quad);
			}

			for (ColliderSphere*& cSphere : sphereObstacles)
			{
				if (particle->Collision(cSphere))
					particle->ResolveCollision(cSphere);
			}
		}
	}

	for (auto& spring : springs)
		spring->Update();

	// Update instance data
	UpdateInstanceData();
}

void Cloth::Render()
{
	instanceBuffer->IASetBuffer(1);
	springBase->RenderInstanced(instanceCount);
}

void Cloth::PostRender()
{
	ImGui::Text("Cloth Options");
	ImGui::Text("Input Keys:");
	ImGui::Text("0 : Init | 1 : ToggleLeftFixed | 2 : ToggleRightFixed");
	ImGui::Text("SpaceBar : TogglePlaying");

	ImGui::Checkbox("IsPlaying",  &isPlaying);
	ImGui::Checkbox("WindActive", &isWindActive);
}

void Cloth::AddObstacles(Transform* obstacle)
{
	ColliderSphere* cSphere = dynamic_cast<ColliderSphere*>(obstacle);

	if (cSphere)
	{
		sphereObstacles.push_back(cSphere);
		return;
	}

	Quad* quad = dynamic_cast<Quad*>(obstacle);

	if (quad)
	{
		quadObstacles.push_back(quad);
		return;
	}

}

void Cloth::InitSpringsAndParticles()
{
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			RigidSphere* particle = new RigidSphere(0.01f, 0.1f);
			particle->translation = { (x - 10) * 2.f, 200.f - (y - 1) * 2.f, Random(0.f, 1.f) };
			//particle->SetFixed(true);
			particles.push_back(particle);
		}
	}

	for (int y = 0; y < 20; y++) for (int x = 0; x < 19; x++)
			springs.push_back(new Spring(particles[(y) * 20 + x], particles[(y) * 20 + x + 1]));

	for (int y = 0; y < 19; y++) for (int x = 0; x < 20; x++)
			springs.push_back(new Spring(particles[(y) * 20 + x], particles[(y + 1) * 20 + x]));

	for (int y = 0; y < 19; y++) for (int x = 0; x < 19; x++)
			springs.push_back(new Spring(particles[(y) * 20 + x], particles[(y + 1) * 20 + x + 1], 200.f));

	for (int y = 0; y < 19; y++) for (int x = 0; x < 19; x++)
			springs.push_back(new Spring(particles[(y + 1) * 20 + x], particles[(y) * 20 + x + 1], 200.f));

	particles[FIXED_LEFT_IDX]->SetFixed(true);
	particles[FIXED_RIGHT_IDX]->SetFixed(true);
}

void Cloth::InitInstancing()
{
	instanceCount = springs.size();

	// instancing °ø¿ë spring
	springBase = new Spring;
	springBase->SetShader(L"BasicColorInstancing");

	for (UINT i = 0; i < instanceCount; i++)
	{
		InstanceData data{};

		data.transform = XMMatrixTranspose(springs[i]->GetWorld());
		data.color = color;
		instanceData.emplace_back(data);
	}

	instanceBuffer = new VertexBuffer(instanceData);
}

void Cloth::HandleInput()
{
	if (KEY_DOWN(VK_SPACE)) isPlaying = !isPlaying;

	if (KEY_DOWN('0'))
	{
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
				particles[x + y * 20]->translation = { (x - 10) * 2.f, 200.f - (y - 1) * 2.f, Random(0.f, 1.f) };
		}

		particles[FIXED_LEFT_IDX]->SetFixed(true);
		particles[FIXED_RIGHT_IDX]->SetFixed(true);

		for (RigidSphere* particle : particles) particle->Update();
		for (Spring* spring : springs)			spring->Update();

		UpdateInstanceData();

		isPlaying = false;
	}

	if (KEY_DOWN('1'))	particles[FIXED_LEFT_IDX]->ToggleFixed();
	if (KEY_DOWN('2'))	particles[FIXED_RIGHT_IDX]->ToggleFixed();
}

void Cloth::UpdateInstanceData()
{
	for (UINT i = 0; i < instanceCount; i++)
		instanceData[i].transform = XMMatrixTranspose(springs[i]->GetWorld());

	instanceBuffer->UpdateVertex(instanceData.data(), instanceCount);
}
