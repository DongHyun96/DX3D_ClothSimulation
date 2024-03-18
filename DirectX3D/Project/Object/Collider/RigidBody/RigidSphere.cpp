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
	/*OBBQuad box = other->GetOBB();
	
	Vector3 pos = other->GetGlobalPosition();

	for (UINT i = 0; i < 3; i++)
	{
		float length = Vector3::Dot(box.axis[i], globalPosition - other->GetGlobalPosition());
	
		float mult = (length < 0.f) ? -1.f : 1.f;
	
		length = min(abs(length), box.halfSize[i]);
	
		pos += box.axis[i] * length * mult;
	}
	
	float distance = (globalPosition - pos).Length();
	
	return distance <= this->Radius();*/

	return Vector3::Dot(globalPosition - other->GetGlobalPosition(), other->GetNormal()) < 0.00001f &&
		   Vector3::Dot(velocity, other->GetNormal()) < 0;
}

bool RigidSphere::Collision(const ColliderSphere* other)
{
	Vector3 pos = other->GetGlobalPosition() + (this->globalPosition - other->GetGlobalPosition()).GetNormalized() * other->Radius();
	Vector3 n = (this->globalPosition - other->GetGlobalPosition()).GetNormalized();

	return Vector3::Dot(this->globalPosition - pos, n) < 0.00001f && Vector3::Dot(velocity, n) < 0;
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
	Vector3 contactVec = other->Radius() * (this->globalPosition - other->GetGlobalPosition()).GetNormalized();

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
	AddForce(GRAVITY * mass);
	AddForce(velocity * (-K_DRAG));
}

void RigidSphere::UpdateRigidBody(const float& timeRate)
{
	if (fixed)
	{
		velocity = Vector3();
		return;
	}

	velocity	+= (force / mass) * (DELTA_TIME / timeRate);
	translation +=       velocity * (DELTA_TIME / timeRate);
}
