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
	void ToggleFixed() { fixed = !fixed; }

	Vector3 GetVelocity() const { return velocity; }

public: // �浹�˻� �� �浹ó��

	bool Collision(const Quad* other);

	bool Collision(const ColliderSphere* other);

	void ResolveContact(const ColliderSphere* other, const UINT& timeRate = 1.f);
	void ResolveContact(const RigidSphere*    other, const UINT& timeRate = 1.f);

	void ResolveContact(const Quad* other, const UINT& timeRate = 1.f);

	void ResolveCollision(const ColliderSphere*	other);		// ColliderSphere
	void ResolveCollision(const RigidSphere*	other);		// RigidBody vs RigidBody

	void ResolveCollision(const Quad* other);			
	void ResolveCollision(const Terrain* terrain);			// NOT IMPLEMENTED

private:

	bool fixed = false;

	float	mass{};
	Vector3 velocity{};
	Vector3 force{};

	const float STEP_RATE = 100.f;

};
