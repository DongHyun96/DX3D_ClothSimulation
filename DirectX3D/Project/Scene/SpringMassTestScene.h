#pragma once

class SpringMassTestScene : public Scene
{

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

	Cloth* cloth{};

	//vector<ColliderSphere*> obstacles{};

	map<ColliderSphere*, Sphere*> obstacles{}; // <Collider, Rendering Sphere>

private:

	// Differential equation Ǯ�̿� timeStep -> �� Update tick �ȿ��� dt�� �۰� �ɰ��� �뵵
	// Euler method���� ������ �� velocity�� ��ġ�� ����� ��, raw�� delta time ��� �� ������ ����� Ŀ�� �ùķ��̼��� ��������
	const UINT PHYSICS_TIMESTEP = 1;

};
