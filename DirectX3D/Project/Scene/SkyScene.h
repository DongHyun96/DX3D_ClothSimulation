#pragma once
class SkyScene : public Scene
{
public:
	SkyScene();
	~SkyScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

	SkyBox* sky{};

};
