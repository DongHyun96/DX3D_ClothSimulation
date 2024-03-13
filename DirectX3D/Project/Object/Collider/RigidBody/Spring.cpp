#include "Framework.h"
#include "Spring.h"


Spring::Spring(RigidSphere* p1, RigidSphere* p2, float k)
	:particle1(p1), particle2(p2), r((p1->GetGlobalPosition() - p2->GetGlobalPosition()).Length()), k(k)
{
}

Spring::~Spring()
{
}

void Spring::Update()
{
	AddForce();
}

void Spring::Render()
{

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
