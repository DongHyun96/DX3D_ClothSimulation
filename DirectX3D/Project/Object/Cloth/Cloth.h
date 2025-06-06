﻿#pragma once
class Cloth : public GameObject<VertexColorNormal>
{
	typedef VertexColorNormal VertexType;

private:

	struct InstanceData
	{
		Matrix transform{};

		Vector4 color{};
	};

private:

	void CreateMesh() override;

public:
	Cloth(Vector4 color = Vector4(1, 0, 0, 1));
	~Cloth();

	void Update() override;;
	void Render(D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) override;
	void PostRender();

	void AddObstacles(Transform* obstacle);

private:

	void InitObjects();
	void InitSpringInstancing();
	void InitSimulation();

	void HandleInput();

	void UpdateSpringInstanceData();

	void UpdateFabricMesh();

	void UpdateWindActive();

private:

	Vector4				 color{ 1, 0, 0, 1 };

	vector<RigidSphere*> particles{};
	vector<Spring*>		 springs{}; // Update용 spring들

	static const UINT	FIXED_LEFT_IDX;
	static const UINT	FIXED_RIGHT_IDX;

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
	
	Vector3		windVelocity	= Vector3(0, 0, 3.f);
	float		accelSignTimer{};
	int			accelSign		= 1;
	float		windBias		= 1.f;

	static const float ACCEL_AMOUNT;
	
private:

	enum Mode
	{
		RAW_SPRING,
		FABRIC
	} mode = FABRIC;

private:

	DifferentialEquationSolver SolverType = RUNGE_KUTTA;
	
};
