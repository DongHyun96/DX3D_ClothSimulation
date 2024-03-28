#pragma once
class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	void Update()		override;
	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

	SkyBox* sky{};

	TextureCube* defaultFloor{};
};
