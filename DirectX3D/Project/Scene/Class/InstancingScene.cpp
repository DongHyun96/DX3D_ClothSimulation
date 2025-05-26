#include "Framework.h"
#include "InstancingScene.h"


InstancingScene::InstancingScene()
{
	//for (float x = 0; x < COUNT; x++)
	//{
	//	for (float y = 0; y < COUNT; y++)
	//	{
	//		Quad* quad = new Quad;

	//		quad->GetMaterial()->SetDiffuseMap(L"Landscape/Box.png");
	//		quad->translation = { 2 * x, 2 * y, 1 };

	//		quads.emplace_back(quad);
	//	}
	//}

	quad = new Quad;
	quad->GetMaterial()->SetDiffuseMap(L"Landscape/Box.png");
	quad->GetMaterial()->SetShader(L"12_TextureInstancing");

	for (float x = 0; x < COUNT; x++)
	{
		for (float y = 0; y < COUNT; y++)
		{
			InstanceData data{};

			data.transform	= XMMatrixTranspose(XMMatrixTranslation(2 * x, 2 * y, 1));
			data.color		= Vector4(x / COUNT, y / COUNT, (x + y) / (2 * COUNT), 1);
			instanceData.emplace_back(data);
		}
	}

	instanceBuffer = new VertexBuffer(instanceData);

}

InstancingScene::~InstancingScene()
{
	//for (Quad* quad : quads)
	//	delete quad;

	delete quad;
	delete instanceBuffer;
}

void InstancingScene::Update()
{
	//for (Quad* quad : quads)
	//	quad->Update();

}

void InstancingScene::Render()
{
	//for (Quad* quad : quads)
	//	quad->Render();

	instanceBuffer->IASetBuffer(1);
	quad->RenderInstanced(COUNT * COUNT);
}

void InstancingScene::PreRender()
{
}

void InstancingScene::PostRender()
{
}
