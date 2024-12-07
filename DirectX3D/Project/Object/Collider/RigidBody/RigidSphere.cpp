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
	return Vector3::Dot(globalPosition - other->GetGlobalPosition(), other->GetNormal()) < 0.01f &&
		   Vector3::Dot(velocity, other->GetNormal()) < 0;
}

bool RigidSphere::Collision(const ColliderSphere* other)
{
	// z fighting 때문에 radius에 여유를 둠
	float otherRadius = other->Radius() + SPHERE_COLLISION_MARGIN;

	Vector3 pos = other->GetGlobalPosition() + (this->globalPosition - other->GetGlobalPosition()).GetNormalized() * otherRadius;
	Vector3 n	= (this->globalPosition - other->GetGlobalPosition()).GetNormalized();

	return Vector3::Dot(this->globalPosition - pos, n) < 0.01f && Vector3::Dot(velocity, n) < 0;
}

void RigidSphere::ResolveContact(const ColliderSphere* other, const UINT& timeRate)
{
	Vector3 n = (this->globalPosition - other->GetGlobalPosition()).GetNormalized();
	float dt = DELTA_TIME / timeRate;


	Vector3 fN = Vector3::Dot(force, n) * n;
	Vector3 vN = Vector3::Dot(velocity, n) * n;
	Vector3 vT = velocity - vN;
	Vector3 fF = -MU * fN.Length() * vT.GetNormalized();

	if (fF.Length() * dt > vT.Length() * mass)
		fF = -vT / dt * mass;

	AddForce(fF);
	AddForce(-fN);
}

void RigidSphere::ResolveContact(const RigidSphere* other, const UINT& timeRate)
{
	ResolveContact((ColliderSphere*)other, timeRate);
}

void RigidSphere::ResolveContact(const Quad* other, const UINT& timeRate)
{
	Vector3 planeNormal = other->GetNormal();
	float dt = DELTA_TIME / timeRate;

	Vector3 fN = Vector3::Dot(force, planeNormal) * planeNormal;
	Vector3 vN = Vector3::Dot(velocity, planeNormal) * planeNormal;
	Vector3 vT = velocity - vN;
	Vector3 fF = -MU * fN.Length() * vT.GetNormalized();

	if (fF.Length() * dt > vT.Length() * mass)
		fF = -vT / dt * mass;

	AddForce(fF);
	AddForce(-fN);
}

void RigidSphere::ResolveCollision(const ColliderSphere* other)
{
	float otherRadius = other->Radius() + SPHERE_COLLISION_MARGIN;

	Vector3 contactVec = otherRadius * (this->globalPosition - other->GetGlobalPosition()).GetNormalized();

	Vector3 contactPos	= other->GetGlobalPosition() + contactVec;
	Vector3 n			= (this->globalPosition - other->GetGlobalPosition()).GetNormalized();

	Vector3 vN	= Vector3::Dot(velocity, n) * n;
	Vector3 vT	= velocity - vN;

	velocity	= vT - vN * COR;
	
	this->translation -= Vector3::Dot(this->translation - contactPos, n) * n;
}

void RigidSphere::ResolveCollision(const RigidSphere* other)
{
	ResolveCollision((ColliderSphere*)other);
}

void RigidSphere::ResolveCollision(const Quad* other)
{
	Vector3 vN = Vector3::Dot(velocity, other->GetNormal()) * other->GetNormal();
	Vector3 vT = velocity - vN;

	velocity = vT - vN * COR;

	translation -= Vector3::Dot(translation - other->GetGlobalPosition(), other->GetNormal()) * other->GetNormal();
}

void RigidSphere::ResolveCollision(const Terrain* terrain)
{
}

void RigidSphere::AddVelocity()
{
	AddForce(GRAVITY * mass); // Gravity force
	AddForce(velocity * (-K_DRAG));
}

void RigidSphere::UpdateRigidBody(const float& timeRate)
{
	if (fixed)
	{
		velocity = Vector3();
		return;
	}

	// Euler integration
	//velocity	+= (force / mass) * (DELTA_TIME / timeRate);
	//translation +=       velocity * (DELTA_TIME / timeRate);

	// Runge-Kutta integration
	Vector3 k1 = (force / mass) * (DELTA_TIME / timeRate);
	Vector3 k2 = ((force / mass) + 0.5f * k1) * (DELTA_TIME / timeRate);
	Vector3 k3 = ((force / mass) + 0.5f * k2) * (DELTA_TIME / timeRate);
	Vector3 k4 = ((force / mass) + k3) * (DELTA_TIME / timeRate);

	velocity	+= (k1 + 2.f * k2 + 2.f * k3 + k4) / 6.f;
	translation += velocity * (DELTA_TIME / timeRate);
}
