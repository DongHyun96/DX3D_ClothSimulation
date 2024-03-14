#pragma once
class Spring
{
public:
	Spring(RigidSphere* p1, RigidSphere* p2, float k = DEFAULT_K_SPRING, Vector4 color = {1, 0, 0, 1});
	~Spring();

	void Update();
	void Render();

	void AddForceToParticles();

	void Debug();

private:

	void UpdateBody();

private:

	Cube* body{};
	Vector4 color{};

	RigidSphere* particle1{};
	RigidSphere* particle2{};

	float r{}; 
	float k{};

};
