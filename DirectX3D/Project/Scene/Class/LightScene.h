#pragma once
class LightScene : public Scene
{
public:
	LightScene();
	~LightScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

	Quad* floor{};

	ModelAnimator* knight{};

	Sphere* sphere{};

	DepthStencil* depthStencil{};
	RenderTarget* renderTarget{};
};
