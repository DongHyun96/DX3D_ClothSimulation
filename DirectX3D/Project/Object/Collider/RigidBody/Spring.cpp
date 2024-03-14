#include "Framework.h"
#include "Spring.h"


Spring::Spring(RigidSphere* p1, RigidSphere* p2, Vector4 color, float k)
	:particle1(p1), particle2(p2), color(color), r((p1->GetGlobalPosition() - p2->GetGlobalPosition()).Length()), k(k)
{
	body = new Cube;
	body->scale.x = 0.5f;
	body->scale.z = 0.5f;

	UpdateBody();
}

Spring::~Spring()
{
	delete body;
}

void Spring::Update()
{
	r = (particle1->GetGlobalPosition() - particle2->GetGlobalPosition()).Length();
	AddForce();
	UpdateBody();
}

void Spring::Render()
{
	body->Render();
}

void Spring::AddForce()
{
	// Using spring force law
	Vector3 p1_to_p2 = particle2->GetGlobalPosition() - particle1->GetGlobalPosition();

	Vector3 F = -(k * (p1_to_p2.Length() - r) +
		        K_D *
				Vector3::Dot(particle2->GetVelocity() - particle1->GetVelocity(), p1_to_p2.GetNormalized())) *
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

	body->translation	 = particle1->GetGlobalPosition() + p1_to_p2 * 0.5f;

	Vector3 rotationAxis = Vector3::Cross(Vector3(0, 1, 0), p1_to_p2);
	float	rotAngle	 = acosf(Vector3::Dot(Vector3(0, 1, 0), p1_to_p2.GetNormalized()));

	body->SetRotationMatrix(rotationAxis, rotAngle);

	body->scale.y = p1_to_p2.Length();

	body->Update();

}
