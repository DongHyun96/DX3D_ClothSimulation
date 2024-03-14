#pragma once
class Spring
{
public:
	Spring(RigidSphere* p1, RigidSphere* p2, Vector4 color = {1, 0, 0, 1}, float k = DEFAULT_K_SPRING);
	~Spring();

	void Update();
	void Render();

	void AddForce();

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
