#pragma once
class TerrainLODScene : public Scene
{
public:
	TerrainLODScene();
	~TerrainLODScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

	TerrainLOD* terrain{};
};
