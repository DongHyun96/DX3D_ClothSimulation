#pragma once

/*
	시도해볼만한 최적화

	1. Release

	2. Rigid Sphere & Spring instancing
	-> Update 최적화까지 해야 될듯?

	프레임 확보되면 step을 적게 주어도 되는지

*/


class SpringMassTestScene : public Scene
{
	struct InstanceData
	{
		Matrix transform{};

		Vector4 color{};
	};

public:
	SpringMassTestScene();
	~SpringMassTestScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

	void Init();

private:

	Quad* floor{};

	vector<RigidSphere*>	particles{};
	vector<Spring*>			springs{};

	ColliderSphere* obstacle{};

	/*
	//vector<Quad*> quads{};

	const UINT COUNT = 50;

	Quad* quad{}; // 공용 quad 하나

	vector<InstanceData> instanceData{};

	VertexBuffer* instanceBuffer{};
	*/

	const UINT FIXED_LEFT_IDX = 0;
	const UINT FIXED_RIGHT_IDX = 19;

	bool isPlaying = true;

};
