#pragma once
class Cloth
{
public:
	Cloth();
	~Cloth();

	void Update();
	void Render();

	void AddObstacles(Transform* obstacle);

private:

	void InitSpringsAndParticles();
	void HandleInput();

private:

	vector<RigidSphere*> particles{};
	vector<Spring*>		 springs{};

	const UINT FIXED_LEFT_IDX  = 0;
	const UINT FIXED_RIGHT_IDX = 19;

	bool isPlaying = true;

private: // 충돌 처리 검사용

	vector<Quad*>			quadObstacles{};
	vector<ColliderSphere*> sphereObstacles{};
	
	
};
