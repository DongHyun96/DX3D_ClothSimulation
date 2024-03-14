#pragma once
class RigidBodyTestScene : public Scene
{
public:
	RigidBodyTestScene();
	~RigidBodyTestScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

	void HandleInput();

private:
	//Quad* floor{};
	vector<Quad*> floors{};

	//RigidSphere* rigidSphere{};

	vector<RigidSphere*> rigidSpheres{};

	ColliderSphere* obstacle{};
};
