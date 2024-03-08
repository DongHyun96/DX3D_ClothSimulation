#include "Framework.h"
#include "BillboardScene.h"


BillboardScene::BillboardScene()
{
	terrain = new Terrain(L"HeightMap/HeightMap.png");
	terrain->GetMaterial()->SetDiffuseMap(L"Landscape/Dirt2.png");
	terrain->translation.y = 1.f;

	//for (UINT i = 0; i < 10000; i++)
	//{
	//	Vector2 size = Vector2(5, 5);

	//	size.x = Random(3.f, 10.f);
	//	size.y = Random(3.f, 10.f);

	//	Vector3 pos{};

	//	pos.x = Random(0.f, terrain->GetSize().x);
	//	pos.z = Random(0.f, terrain->GetSize().y);

	//	pos.y = terrain->GetHeight(pos) + size.y * 0.5f + 1.f;

	//	Quad* quad = new Quad(size);
	//	quad->GetMaterial()->SetDiffuseMap(L"Landscape/Tree.png");
	//	quad->GetMaterial()->SetShader(L"01_Texture");
	//	//quad->GetMaterial()->SetShader(L"21_Billboard");


	//	quad->translation = pos;

	//	trees.emplace_back(quad);
	//}

	material = new Material;
	material->SetShader(L"21_Billboard");
	material->SetDiffuseMap(L"Landscape/Tree.png");

	geometryShader = Shader::AddGS(L"21_Billboard");

	for (UINT i = 0; i < MAX_COUNT; i++)
	{
		Vector2 size = Vector2(5, 5);

		size.x = Random(3.f, 10.f);
		size.y = Random(3.f, 10.f);

		Vector3 pos{};

		pos.x = Random(0.f, terrain->GetSize().x);
		pos.z = Random(0.f, terrain->GetSize().y);

		pos.y = terrain->GetHeight(pos) + size.y * 0.5f + 1.f;

		// 원래는 quad로 그렸지만 Vertex 정보에 pos와 size만 넣어줌
		VertexTexture vertex;
		vertex.pos = pos;
		vertex.uv = size;

		vertices.emplace_back(vertex);
	}

	//drawVertices

	vertexBuffer = new VertexBuffer(vertices);
}

BillboardScene::~BillboardScene()
{
	delete terrain;

	/*for (Quad* tree : trees)
		delete tree;*/

	delete material;
	delete vertexBuffer;
}

void BillboardScene::Update()
{
	terrain->Update();

	//for (Quad* tree : trees)
	//{
	//	//tree->rotation.y = CAMERA->rotation.y;

	//	Vector3 dir =  tree->GetGlobalPosition() - CAMERA->GetGlobalPosition();
	//	
	//	float angle = atan2f(dir.x, dir.z); // 회전할 각

	//	tree->rotation.y = angle;

	//	tree->Update();
	//}

}

void BillboardScene::Render()
{
	terrain->Render();

	//STATE->EnableAlpha();
	STATE->EnableAlphaToCoverage();

	/*for (Quad* tree : trees)
		tree->Render();*/

	vertexBuffer->IASetBuffer(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	material->Set();
	geometryShader->SetShader();

	DC->Draw(drawCount, 0); // 나무들을 그리는 drawCall

	DC->GSSetShader(nullptr, nullptr, 0); // GS 초기화

	STATE->DisableAlpha();

}

void BillboardScene::PreRender()
{
}

void BillboardScene::PostRender()
{
}
