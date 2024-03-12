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
	/*
	// Using spring force law
		glm::vec3 F = -(k * (length(particle2.position - particle1.position) - r) + K_D * dot(particle2.velocity - particle1.velocity, normalize(particle2.position - particle1.position))) * normalize(particle2.position - particle1.position);
		
		particle1.addForce(-F);
		particle2.addForce(F);
	*/

	/*Vector3 F = -(k * (particle2->GetGlobalPosition() - particle1->GetGlobalPosition()).Length()- r) +
		        K_D * Vector3::Dot(particle2->GetVelocity() - particle1->GetVelocity(), GetNormalized(particle2->GetGlobalPosition() - particle1->GetGlobalPosition()))*/
}
