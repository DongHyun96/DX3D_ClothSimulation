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

	Cloth* cloth{};

	vector<ColliderSphere*> obstacles{};

};
