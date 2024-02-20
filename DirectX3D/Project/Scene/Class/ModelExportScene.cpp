#include "Framework.h"
#include "ModelExportScene.h"


ModelExportScene::ModelExportScene()
{
	exporter = new ModelExporter("Sword");
	exporter->ExportModel();
	//exporter->ExportAnimation("Sad Idle");
}

ModelExportScene::~ModelExportScene()
{
	delete exporter;
}

void ModelExportScene::Update()
{
}

void ModelExportScene::Render()
{
}

void ModelExportScene::PreRender()
{
}

void ModelExportScene::PostRender()
{
}
