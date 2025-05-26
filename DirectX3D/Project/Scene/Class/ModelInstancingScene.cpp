#include "Framework.h"
#include "ModelInstancingScene.h"


ModelInstancingScene::ModelInstancingScene()
{
	model = new ModelAnimatorInstancing("Knight");

	model->ReadClip("Sad Idle");
	model->ReadClip("Goofy Running");
	model->ReadClip("Dancing Twerk");


	for (float x = 0; x < 10; x++)
	{
		for (float z = 0; z < 10; z++)
		{
			Transform* transform = model->AddTransform();

			transform->translation = { x * 10, 0, z * 10 };
			transform->scale *= 0.05f;
		}
	}

	sky = new SkyBox(L"Landscape/ddsTest.dds");
}

ModelInstancingScene::~ModelInstancingScene()
{
	delete model;

	delete sky;
}

void ModelInstancingScene::Update()
{
	model->Update();
	sky->Update();
}

void ModelInstancingScene::Render()
{
	sky->Render();
	model->Render();
}

void ModelInstancingScene::PreRender()
{
}

void ModelInstancingScene::PostRender()
{
	model->Debug();
}

