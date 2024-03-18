#pragma once

/*
	�õ��غ����� ����ȭ

	1. Release

	2. Rigid Sphere & Spring instancing
	-> Update ����ȭ���� �ؾ� �ɵ�?

	������ Ȯ���Ǹ� step�� ���� �־ �Ǵ���

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

	Quad* quad{}; // ���� quad �ϳ�

	vector<InstanceData> instanceData{};

	VertexBuffer* instanceBuffer{};
	*/

	const UINT FIXED_LEFT_IDX = 0;
	const UINT FIXED_RIGHT_IDX = 19;

	bool isPlaying = true;

};
