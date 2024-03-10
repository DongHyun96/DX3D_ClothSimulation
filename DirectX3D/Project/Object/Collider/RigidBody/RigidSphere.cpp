#include "Framework.h"
#include "RigidSphere.h"



RigidSphere::RigidSphere(float mass, float radius, UINT stackCount, UINT sliceCount)
	:ColliderSphere(radius, stackCount, sliceCount), mass(mass)
{
}

RigidSphere::~RigidSphere()
{
}

void RigidSphere::Update()
{
	ColliderSphere::Update();

	ClearForce();

	AddForce(GRAVITY * mass);
	AddForce(velocity * (-K_DRAG));

	UpdateRigidBody();
}

void RigidSphere::Render()
{
	ColliderSphere::Render();
}

void RigidSphere::ClearForce()
{
	force = Vector3();
}

bool RigidSphere::Collision(const Quad* other)
{
	return Vector3::Dot(globalPosition - other->GetGlobalPosition(), other->GetNormal()) < 0.00001f &&
		   Vector3::Dot(velocity, other->GetNormal()) < 0;
}

void RigidSphere::HandleCollision(const ColliderSphere* other)
{
	Vector3 contactVec	= other->Radius() * (this->globalPosition - other->GetGlobalPosition().GetNormalized());
	Vector3 contactPos	= other->GetGlobalPosition() + contactVec;
	Vector3 n			= (this->globalPosition - other->GetGlobalPosition()).GetNormalized();

	Vector3 vN	= Vector3::Dot(velocity, n) * n;
	Vector3 vT	= velocity - vN;
	velocity	= vT - vN * COR;

	this->translation -= Vector3::Dot(translation - contactPos, n) * n;
}

void RigidSphere::HandleCollision(const RigidSphere* other)
{
	HandleCollision((ColliderSphere*)other);
}

void RigidSphere::HandleCollision(const Quad* other)
{
	Vector3 vN = Vector3::Dot(velocity, other->GetNormal()) * other->GetNormal();
	Vector3 vT = velocity - vN;

	velocity = vT - vN * COR;

	translation -= Vector3::Dot(translation - other->GetGlobalPosition(), other->GetNormal()) * other->GetNormal();
}

void RigidSphere::HandleCollision(const Terrain* terrain)
{
}

void RigidSphere::UpdateRigidBody()
{
	if (fixed)
	{
		velocity = Vector3();
		return;
	}

	velocity	+= (force / mass) * (DELTA_TIME);
	translation += velocity * (DELTA_TIME);
}
