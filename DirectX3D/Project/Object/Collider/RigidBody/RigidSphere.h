#pragma once
/// <summary>
/// ������ �� : �ֻ��� Transform���� �ξ�� ���� �۵� -> local Transform�� �� global Transform�� �Ǿ�� ��
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

public: // �浹�˻� �� �浹ó��

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
