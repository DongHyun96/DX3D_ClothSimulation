#pragma once
class TerrainScene : public Scene
{
public:
	TerrainScene();
	~TerrainScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:
	
	Terrain* terrain{};

	Cube* cube{};
};
