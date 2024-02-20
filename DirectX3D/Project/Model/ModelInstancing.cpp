#include "Framework.h"
#include "ModelInstancing.h"


ModelInstancing::ModelInstancing(string name)
	:Model(name)
{
	instanceDatas.resize(MAX_INSTANCE);

	instanceBuffer = new VertexBuffer(instanceDatas);

	SetShader(L"13_ModelInstancing");

}

ModelInstancing::~ModelInstancing()
{
	delete instanceBuffer;

	for (Transform* transform : transforms)
		delete transform;

	transforms.clear();
}

void ModelInstancing::Update()
{
	drawCount = 0;

	for (UINT i = 0; i < transforms.size(); i++)
	{
		if (!transforms[i]->GetIsActive()) continue;

		transforms[i]->Update();

		instanceDatas[drawCount].world = XMMatrixTranspose(transforms[i]->GetWorld());
		instanceDatas[drawCount].index = i;

		drawCount++;
	}

	instanceBuffer->UpdateVertex(instanceDatas.data(), drawCount);
}

void ModelInstancing::Render()
{
	instanceBuffer->IASetBuffer(1);

	__super::RenderInstanced(drawCount);
}

void ModelInstancing::Debug()
{
	ImGui::Text("DrawCount : %d", drawCount);

	if (ImGui::Button("Add Model"))
		AddTransform();

	for (Transform* transform : transforms)
		transform->Debug();

}

Transform* ModelInstancing::AddTransform()
{
	Transform* transform = new Transform;

	transform->SetName(name + "_" + to_string(transforms.size()));

	transforms.emplace_back(transform);

	return transform;
}
