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

	Quad* floor{};

	//vector<RigidSphere*>	particles{};
	//vector<Spring*>			springs{};

	RigidSphere* p1{};
	RigidSphere* p2{};
	Spring* spring{};

};
