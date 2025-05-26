#include "Framework.h"
#include "Cloth.h"


Cloth::Cloth(Vector4 color)
	:GameObject<VertexType>(L"02_DiffuseColor"), color(color)
{
	InitObjects();
	InitSpringInstancing();

	CreateMesh();

	this->Update(1);

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


void Cloth::CreateMesh()
{
	vertices.clear();
	indices.clear();

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			//RigidSphere* particle = new RigidSphere(0.01f, 0.1f);
			//particle->translation = { (x - 10) * 2.f, 200.f - (y - 1) * 2.f, Random(0.f, 1.f) };
			////particle->SetFixed(true);
			//particles.push_back(particle);
			VertexType vertex{};
			
			vertex.pos	 = particles[20 * y + x]->GetGlobalPosition();
			vertex.color = color;

			vertices.emplace_back(vertex);
		}
	}

	for (UINT i = 0; i < 20 - 1; i++)
	{
		for (UINT j = 0; j < 20 - 1; j++)
		{
			indices.emplace_back((i + 0) * 20 + (j + 0));
			indices.emplace_back((i + 0) * 20 + (j + 1));
			indices.emplace_back((i + 1) * 20 + (j + 0));
										   
			indices.emplace_back((i + 1) * 20 + (j + 0));
			indices.emplace_back((i + 0) * 20 + (j + 1));
			indices.emplace_back((i + 1) * 20 + (j + 1));
		}
	}

	// Create Normal Vector

	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v01 = vertices[index1].pos - vertices[index0].pos;
		Vector3 v02 = vertices[index2].pos - vertices[index0].pos;

		Vector3 normal = Vector3::Cross(v01, v02).GetNormalized();

		// 추후 평균을 냄
		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
	}

	mesh = new Mesh(vertices, indices);
}

/*
	for Step Rate times...
		Clear All forces
		Particle Add force(Gravity, own velocity)
		Spring Add force to each Particles
		Particle steps(UpdateRigidBody)
		ResolveCollisions
*/
void Cloth::Update(const UINT& PhysicsTimeStep)
{
	if (DELTA_TIME > 0.01f) return;

	HandleInput();

	if (!isPlaying) return;

	for (UINT i = 0; i < PhysicsTimeStep; i++)
	{
		if (isWindActive)
		{
			windVelocity.z += ACCEL_AMOUNT * accelSign * DELTA_TIME / PhysicsTimeStep;

			accelSignTimer += DELTA_TIME / PhysicsTimeStep;

			if (accelSignTimer >= 1.f)
			{
				accelSignTimer -= 1.f;
				accelSign *= -1;
			}
		}

		for (auto& particle : particles)
		{
			particle->ClearForce();

			//particle->AddVelocity();

			particle->AddGravityForce();
			particle->AddViscousDragForce();

			if (isWindActive)
			{
				windBias = Random(-5.f, 10.f);
				particle->AddForce(windVelocity * windBias);
			}
		}

		for (auto& spring : springs)
			spring->AddForceToParticles();

		for (auto& particle : particles)
		{
			particle->SolveCurrentPosition(PhysicsTimeStep, RUNGE_KUTTA);
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

	/* Render 용 Update 과정. Cloth의 각 Particles의 GlobalPosition에 따른 각 도형 Transform 잡기 */

	switch (mode)
	{
	case Cloth::RAW_SPRING:
		// Updating spring transforms
		for (auto& spring : springs)
			spring->Update();

		// Update instance data
		UpdateSpringInstanceData();
		break;
	case Cloth::FABRIC: // Update fabric's vertices pos and normal

		UpdateFabricMesh();

		break;
	default:
		break;
	}
	
}

void Cloth::Render(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	switch (mode)
	{
	case Cloth::RAW_SPRING:

		instanceBuffer->IASetBuffer(1);
		springBase->RenderInstanced(instanceCount);
		return;

	case Cloth::FABRIC:

		STATE->DisableBackFaceCulling();
		__super::Render(topology);
		STATE->EnableBackFaceCulling();

		return;

	default:
		break;
	}
	
}

void Cloth::PostRender()
{
	ImGui::Text("Cloth Options");
	ImGui::Text("Input Keys:");
	ImGui::Text("0 : Init | 1 : ToggleLeftFixed | 2 : ToggleRightFixed");
	ImGui::Text("SpaceBar : TogglePlaying");

	ImGui::Checkbox("IsPlaying",  &isPlaying);
	ImGui::Checkbox("WindActive", &isWindActive);

	const char* list[] = { "Raw Spring", "Fabric" };

	ImGui::Combo("ClothType", (int*)&mode, list, 2);
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

void Cloth::InitObjects()
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

void Cloth::InitSpringInstancing()
{
	instanceCount = springs.size();

	// instancing 공용 spring
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

		UpdateSpringInstanceData();
		UpdateFabricMesh();

		isPlaying = false;
	}

	if (KEY_DOWN('1'))	particles[FIXED_LEFT_IDX]->ToggleFixed();
	if (KEY_DOWN('2'))	particles[FIXED_RIGHT_IDX]->ToggleFixed();
}

void Cloth::UpdateSpringInstanceData()
{
	for (UINT i = 0; i < instanceCount; i++)
		instanceData[i].transform = XMMatrixTranspose(springs[i]->GetWorld());

	instanceBuffer->UpdateVertex(instanceData.data(), instanceCount);
}

void Cloth::UpdateFabricMesh()
{
	// Update pos
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			VertexType& vertex = vertices[20 * y + x];

			vertex.pos = particles[20 * y + x]->GetGlobalPosition();
			vertex.normal = Vector3(0, 1, 0);
		}
	}

	// Update normal
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v01 = vertices[index1].pos - vertices[index0].pos;
		Vector3 v02 = vertices[index2].pos - vertices[index0].pos;

		Vector3 normal = Vector3::Cross(v01, v02).GetNormalized();

		// 추후 평균을 냄
		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
	}

	mesh->UpdateVertex(vertices.data(), vertices.size());
}
