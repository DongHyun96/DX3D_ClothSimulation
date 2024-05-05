#pragma once
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

	void Update();
	void Render(D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) override;
	void PostRender();

	void AddObstacles(Transform* obstacle);

private:

	void InitObjects();
	void InitSpringInstancing();

	void HandleInput();

	void UpdateSpringInstanceData();

	void UpdateFabricMesh();

private:

	// TODO debug / Release ������
//#ifdef _DEBUG
//	//const UINT PHYSICS_TIMESTEP = 20;
//#endif
//
//#ifndef _DEBUG
//	const UINT PHYSICS_TIMESTEP = 1;
//#endif
	const UINT PHYSICS_TIMESTEP = 1;

private:

	Vector4				 color{ 1, 0, 0, 1 };

	vector<RigidSphere*> particles{};
	vector<Spring*>		 springs{}; // Update�� spring��

	const UINT			 FIXED_LEFT_IDX  = 0;
	const UINT			 FIXED_RIGHT_IDX = 19;

	bool				 isPlaying = true;

private: // Instancing

	UINT					instanceCount{};

	Spring*					springBase{}; // Instancing ���� spring
	vector<InstanceData>	instanceData{};
	VertexBuffer*			instanceBuffer{};


private: // �浹 ó�� �˻��

	vector<Quad*>			quadObstacles{};
	vector<ColliderSphere*> sphereObstacles{};
	
private: // �ٶ� ����

	bool		isWindActive{};
	
	Vector3		windVelocity	= Vector3(0, 0, 3.f);
	float		accelSignTimer{};
	int			accelSign		= 1;
	const float ACCEL_AMOUNT	= 1.5f;
	float		bias			= 1.f;

private:

	enum Mode
	{
		RAW_SPRING,
		FABRIC
	} mode{};

};
