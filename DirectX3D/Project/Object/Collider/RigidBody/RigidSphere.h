#pragma once

enum DifferentialEquationSolver
{
	EULER,
	RUNGE_KUTTA
};

/// <summary>
/// 주의할 점 : 최상위 Transform으로 두어야 정상 작동 -> local Transform이 곧 global Transform이 되어야 함
/// </summary>
class RigidSphere : public ColliderSphere
{
public:
	RigidSphere(float mass, float radius = 1, UINT stackCount = 15, UINT sliceCount = 30);
	~RigidSphere();

	void Update();
	void Render();

	void ClearForce();

	void AddForce(const Vector3& force) { this->force += force; }

	void AddGravityForce()     { AddForce(GRAVITY * mass); }
	void AddViscousDragForce() { AddForce(velocity * (-K_DRAG)); }

	/// <summary>
	/// 현재 위치 translation 계산하기
	/// </summary>
	/// <param name="timeStep"></param>
	void SolveCurrentPosition(const UINT& timeStep = 1, const DifferentialEquationSolver& SolverType = RUNGE_KUTTA); // Step

	void SetFixed(const bool& fixed) { this->fixed = fixed; }
	void ToggleFixed() { fixed = !fixed; }

	Vector3 GetVelocity() const { return velocity; }

public: // 충돌검사 및 충돌처리

	bool Collision(const Quad* other);

	bool Collision(const ColliderSphere* other);

	void ResolveContact(const ColliderSphere* other, const UINT& timeStep = 1.f);
	void ResolveContact(const RigidSphere*    other, const UINT& timeStep = 1.f);

	void ResolveContact(const Quad* other, const UINT& timeStep = 1.f);

	void ResolveCollision(const ColliderSphere*	other);		// ColliderSphere
	void ResolveCollision(const RigidSphere*	other);		// RigidBody vs RigidBody

	void ResolveCollision(const Quad* other);			
	void ResolveCollision(const Terrain* terrain);			// NOT IMPLEMENTED

private:

	bool fixed = false;

	float	mass{};
	Vector3 velocity{};
	Vector3 force{};

private:

	const float SPHERE_COLLISION_MARGIN = 0.5f;

};
