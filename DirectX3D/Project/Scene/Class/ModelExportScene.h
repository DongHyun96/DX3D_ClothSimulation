#pragma once
class ModelExportScene : public Scene
{
public:
	ModelExportScene();
	~ModelExportScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

	ModelExporter* exporter{};

};
