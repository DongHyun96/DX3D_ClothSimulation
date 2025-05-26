#pragma once
class WaterScene : public Scene
{
public:
	WaterScene();
	~WaterScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:
	void CreateObjects();

private:

	Quad* floor{};

	ModelAnimator* knight{};

	Sphere* sphere{};

	Model* bunny{};

	//Reflection* reflection{};
	//Refraction* refraction{};

	Water* water{};

	SkyBox* sky{};

};