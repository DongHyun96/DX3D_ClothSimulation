#include "Framework.h"
#include "Spring.h"


Spring::Spring(RigidSphere* p1, RigidSphere* p2, float k, Vector4 color)
	:particle1(p1),
	 particle2(p2),
	 k(k),
	 color(color)
{
	body = new Cube;
	body->GetMaterial()->GetBuffer()->data.diffuse = color;
	//body->GetMaterial()->SetShader(L"16_Light");

	p1->UpdateWorldMatrix();
	p2->UpdateWorldMatrix();

	float s = min(p1->Radius(), p2->Radius());

	body->scale.x = 0.2f;
	body->scale.z = 0.2f;

	r = (p1->GetGlobalPosition() - p2->GetGlobalPosition()).Length();

	UpdateBody();
}

Spring::~Spring()
{
	delete body;
}

void Spring::Update()
{
	UpdateBody();
}

void Spring::Render()
{
	body->Render();
}

void Spring::AddForceToParticles()
{
	// Using spring force law
	Vector3 p1_to_p2 = particle2->GetGlobalPosition() - particle1->GetGlobalPosition();

	Vector3 F = -(k * (p1_to_p2.Length() - r) +
		        K_D * Vector3::Dot(particle2->GetVelocity() - particle1->GetVelocity(), p1_to_p2.GetNormalized())) *
		        (p1_to_p2).GetNormalized();

	particle1->AddForce(-F);
	particle2->AddForce(F);
}

void Spring::Debug()
{
	body->Debug();
}

void Spring::UpdateBody()
{
	Vector3 p1_to_p2	 = particle2->GetGlobalPosition() - particle1->GetGlobalPosition();

	// Update translation
	body->translation	 = particle1->GetGlobalPosition() + p1_to_p2 * 0.5f;

	// Update rotation
	Vector3 rotationAxis = Vector3::Cross(Vector3(0, 1, 0), p1_to_p2);
	float	rotAngle	 = acosf(Vector3::Dot(Vector3(0, 1, 0), p1_to_p2.GetNormalized()));

	body->SetRotationMatrix(rotationAxis, rotAngle);

	// Update scale
	body->scale.y = p1_to_p2.Length();

	// body Update
	body->Update();

}
