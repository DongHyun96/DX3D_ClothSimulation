#include "Framework.h"
#include "TessellationScene.h"


TessellationScene::TessellationScene()
{
	material = new Material(L"30_Triangle");

	hullShader		= Shader::AddHS(L"30_Triangle");
	domainShader	= Shader::AddDS(L"30_Triangle");

	edgeBuffer		= new FloatValueBuffer();
	insideBuffer	= new FloatValueBuffer();

	edgeBuffer->data	= { 1, 1, 1, 1 };
	insideBuffer->data	= { 1, 1, 1, 1 };

	vertices =
	{
		{-1.f, -1.f, 0.f},
		{+0.f, +1.f, 0.f},
		{+1.f, -1.f, 0.f}
	};

	vertexBuffer = new VertexBuffer(vertices);
}

TessellationScene::~TessellationScene()
{
	delete material;
	delete vertexBuffer;

	delete edgeBuffer;
	delete insideBuffer;
}

void TessellationScene::Update()
{
}

void TessellationScene::Render()
{
	edgeBuffer->SetHSBuffer(10);
	insideBuffer->SetHSBuffer(11);

	material->Set();

	hullShader->SetShader();
	domainShader->SetShader();

	vertexBuffer->IASetBuffer(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	DC->Draw(vertices.size(), 0);
}

void TessellationScene::PreRender()
{
}

void TessellationScene::PostRender()
{
	ImGui::Text("TessFactor Option");

	ImGui::DragFloat("Edge0", &edgeBuffer->data.values[0]);
	ImGui::DragFloat("Edge1", &edgeBuffer->data.values[1]);
	ImGui::DragFloat("Edge2", &edgeBuffer->data.values[2]);

	ImGui::DragFloat("Inside", &edgeBuffer->data.values[3]);

}