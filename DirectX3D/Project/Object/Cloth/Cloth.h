#pragma once
class Cloth
{
	struct InstanceData
	{
		Matrix transform{};

		Vector4 color{};
	};

public:
	Cloth(Vector4 color);
	~Cloth();

	void Update();
	void Render();
	void PostRender();

	void AddObstacles(Transform* obstacle);

private:

	void InitSpringsAndParticles();
	void InitInstancing();

	void HandleInput();

	void UpdateInstanceData();

private:

	Vector4				 color{ 1, 0, 0, 1 };

	vector<RigidSphere*> particles{};
	vector<Spring*>		 springs{}; // Update용 spring들

	const UINT			 FIXED_LEFT_IDX  = 0;
	const UINT			 FIXED_RIGHT_IDX = 19;

	bool				 isPlaying = true;

private: // Instancing

	UINT					instanceCount{};

	Spring*					springBase{}; // Instancing 공용 spring
	vector<InstanceData>	instanceData{};
	VertexBuffer*			instanceBuffer{};


private: // 충돌 처리 검사용

	vector<Quad*>			quadObstacles{};
	vector<ColliderSphere*> sphereObstacles{};
	
private: // 바람 관련

	bool		isWindActive{};
	
	Vector3		windVelocity = Vector3(0, 0, 3.f);
	float		accelSignTimer{};
	int			accelSign		= 1;
	const float ACCEL_AMOUNT	= 1.5f;
	float bias = 1.f;

	
};
