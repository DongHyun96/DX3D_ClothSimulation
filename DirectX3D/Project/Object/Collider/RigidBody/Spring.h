#pragma once
class Spring
{
public:
	Spring(RigidSphere* p1, RigidSphere* p2, float k = DEFAULT_K_SPRING);
	~Spring();

	void Update();
	void Render();

	void AddForce();

private:

	RigidSphere* particle1{};
	RigidSphere* particle2{};

	float r{};
	float k{};

};
