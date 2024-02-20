#pragma once
class CollisionScene : public Scene
{
public:
	CollisionScene();
	~CollisionScene();

	// Scene을(를) 통해 상속됨
	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

	vector<Collider*> colliders{};

	Quad* crossHair{};

	ProgressBar* hpBar{};
};
