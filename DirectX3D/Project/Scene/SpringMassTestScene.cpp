#include "Framework.h"
#include "SpringMassTestScene.h"


SpringMassTestScene::SpringMassTestScene()
{
	Init();
}

SpringMassTestScene::~SpringMassTestScene()
{
	delete cloth;

	//delete obstacle;
	for (auto& p : obstacles)
	{
		delete p.first;
		delete p.second;
	}

	obstacles.clear();
}

void SpringMassTestScene::Update()
{
	DEFAULT_FLOOR->Update();

	cloth->Update();

	for (auto& p : obstacles)
	{
		p.first->Update();
		p.second->Update();
	}
}

// Draw call - 1882
void SpringMassTestScene::Render()
{
	DEFAULT_FLOOR->Render();

	for (auto& p : obstacles)
		p.second->Render();

	cloth->Render();
}

void SpringMassTestScene::PreRender()
{
}

void SpringMassTestScene::PostRender()
{
	//obstacle->Debug();
	for (auto& p : obstacles)
	{
		p.first->Debug();
		//p.second->GetMaterial()->Debug();
	}


	DEFAULT_FLOOR->Debug();

	cloth->PostRender();

}

void SpringMassTestScene::Init()
{
	for (UINT i = 0; i < 4; i++)
	{
		ColliderSphere* cSphere = new ColliderSphere(10.f);

		cSphere->SetName("SpringMassObstacle_" + to_string(i));
		cSphere->LoadTransform();

		Sphere* renderingSphere = new Sphere(10.f, 30, 30);
		renderingSphere->GetMaterial()->SetName(to_string(i));
		renderingSphere->GetMaterial()->SetSpecularMap(L"Solid/White.png");
		renderingSphere->GetMaterial()->SetShader(L"16_Light");
		renderingSphere->GetMaterial()->LoadMaterial(L"_TextData/SpringMassObstacle" + to_wstring(i) + L".mat");
		renderingSphere->SetParent(cSphere);

		obstacles[cSphere] = renderingSphere;
	}

	//rgb(98, 114, 84)
	//rgb(91, 188, 255)
	cloth = new Cloth(Vector4(0.356f, 0.737f, 1.f, 1));

	cloth->AddObstacles(DEFAULT_FLOOR->GetFrontQuad());
	
	for (auto& p : obstacles)
		cloth->AddObstacles(p.first);

	ENVIRONMENT->GetLightBuffer()->data.lights[0].direction = {0.826, -0.6, 1};

}
