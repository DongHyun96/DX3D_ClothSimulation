#pragma once
class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

	Model* wheel{};
};
