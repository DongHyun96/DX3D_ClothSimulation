#pragma once
class FrustumCullingScene : public Scene
{
public:
	FrustumCullingScene();
	~FrustumCullingScene();


	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

	vector<Sphere*> spheres{};

	const UINT SIZE = 10;

};
