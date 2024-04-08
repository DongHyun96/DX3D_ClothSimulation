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

	TextureCube* defaultFloor{};

	Cloth* cloth{};

	//vector<ColliderSphere*> obstacles{};

	map<ColliderSphere*, Sphere*> obstacles{}; // <Collider, Rendering Sphere>

};
