#include "Framework.h"
#include "RigidSphere.h"

const float RigidSphere::SPHERE_COLLISION_MARGIN = 0.5f;

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

	// pos = ColliderSphere로 부터 RigidSphere로 향하는 방향에 맞닿는 ColliderSphere위의 점
	Vector3 pos = other->GetGlobalPosition() + (this->globalPosition - other->GetGlobalPosition()).GetNormalized() * otherRadius;
	Vector3 n	= (this->globalPosition - other->GetGlobalPosition()).GetNormalized();

	return Vector3::Dot(this->globalPosition - pos, n) < 0.01f && Vector3::Dot(velocity, n) < 0;
}

void RigidSphere::ResolveContact(const ColliderSphere* other, const UINT& timeStep)
{
	Vector3 n = (this->globalPosition - other->GetGlobalPosition()).GetNormalized();
	float dt  = DELTA_TIME / timeStep;


	Vector3 fN = Vector3::Dot(force, n) * n;
	Vector3 vN = Vector3::Dot(velocity, n) * n;
	Vector3 vT = velocity - vN;
	Vector3 fF = -MU * fN.Length() * vT.GetNormalized();

	if (fF.Length() * dt > vT.Length() * mass)
		fF = -vT / dt * mass;

	AddForce(fF);
	AddForce(-fN);
}

void RigidSphere::ResolveContact(const RigidSphere* other, const UINT& timeStep)
{
	ResolveContact((ColliderSphere*)other, timeStep);
}

void RigidSphere::ResolveContact(const Quad* other, const UINT& timeStep)
{
	Vector3 planeNormal = other->GetNormal();
	float dt = DELTA_TIME / timeStep;

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
	// z fighting 문제로 실질적인 radius에 약간의 margin을 더함
	float otherRadius = other->Radius() + SPHERE_COLLISION_MARGIN;

	// other sphere 위치로부터 나 자신으로 향하는 방향
	Vector3 n = (this->globalPosition - other->GetGlobalPosition()).GetNormalized();

	// Velocity update
	Vector3 vN	= Vector3::Dot(velocity, n) * n;
	Vector3 vT	= velocity - vN;
	velocity	= vT - vN * COR;
	
	// Translation 보정
	Vector3 contactVec = otherRadius * n;
	Vector3 contactPos = other->GetGlobalPosition() + contactVec;
	this->translation -= Vector3::Dot(this->translation - contactPos, n) * n;
}

void RigidSphere::ResolveCollision(const RigidSphere* other)
{
	ResolveCollision((ColliderSphere*)other);
}

void RigidSphere::ResolveCollision(const Quad* other)
{
	// Velocity update
	Vector3 vN = Vector3::Dot(velocity, other->GetNormal()) * other->GetNormal();
	Vector3 vT = velocity - vN;
	velocity   = vT - vN * COR;

	// Translation 보정
	translation -= Vector3::Dot(translation - other->GetGlobalPosition(), other->GetNormal()) * other->GetNormal();
}

void RigidSphere::ResolveCollision(const Terrain* terrain)
{
}

void RigidSphere::SolveCurrentPosition(const UINT& timeStep, const DifferentialEquationSolver& SolverType)
{
	if (fixed)
	{
		velocity = Vector3();
		return;
	}

	Vector3 forcePerMass = force / mass;
	float dt = DELTA_TIME / timeStep;

	switch (SolverType)
	{
	case EULER:
		/*Euler integration
		translation(t + dt) = translation(t) + curVelocity * dt;
		dt를 최대한 줄임으로써(timeStep으로) 오차 줄이기 시도*/
		velocity	+= forcePerMass * dt;
		translation += velocity     * dt;
		break;
	case RUNGE_KUTTA:
	{
		// Runge-Kutta integration
		Vector3 k1 = forcePerMass * dt;
		Vector3 k2 = (forcePerMass + 0.5f * k1) * dt;
		Vector3 k3 = (forcePerMass + 0.5f * k2) * dt;
		Vector3 k4 = (forcePerMass + k3) * dt;
	
		velocity	+= (k1 + 2.f * k2 + 2.f * k3 + k4) / 6.f;
		translation += velocity * dt;
	}
		break;
	default: break;
	}
}
