#pragma once
class TerrainEditorScene : public Scene
{
public:
	TerrainEditorScene();
	~TerrainEditorScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

	TerrainEditor* editor{};

};
