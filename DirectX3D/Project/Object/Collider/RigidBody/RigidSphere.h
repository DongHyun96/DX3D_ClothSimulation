#pragma once
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

	void AddVelocity();

	void UpdateRigidBody(const float& timeRate = 1.f); // Step

	void SetFixed(const bool& fixed) { this->fixed = fixed; }

	Vector3 GetVelocity() const { return velocity; }

public: // 충돌검사 및 충돌처리

	bool Collision(const Quad* other);

	void HandleCollision(const ColliderSphere*	other); // ColliderSphere
	void HandleCollision(const RigidSphere*		other); // RigidBody vs RigidBody

	void HandleCollision(const Quad* other);			
	void HandleCollision(const Terrain* terrain);		// NOT IMPLEMENTED

private:

	bool fixed = false;

	float	mass{};
	Vector3 velocity{};
	Vector3 force{};

	const float STEP_RATE = 100.f;

};
