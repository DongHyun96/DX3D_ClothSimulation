#include "Framework.h"
#include "Spark.h"


Spark::Spark(wstring file, bool isAdditive)
{
	this->isAdditive = isAdditive;

	material->SetShader(L"23_Spark");
	material->SetDiffuseMap(file);

	geometryShader = Shader::AddGS(L"23_Spark");

	valueBuffer = new FloatValueBuffer();
	startColorBuffer = new FloatValueBuffer();
	endColorBuffer = new FloatValueBuffer();

	for (UINT i = 0; i < 4; i++)
	{
		startColorBuffer->data.values[i] = 1.f;
		endColorBuffer->data.values[i] = 1.f;
	}

	SetVertex();
}

Spark::~Spark()
{
	delete valueBuffer;
	delete startColorBuffer;
	delete endColorBuffer;
}

void Spark::Update()
{
	if (!isActive) return;

	// value0 : time
	// value1 : duration

	valueBuffer->data.values[0] += DELTA_TIME;

	if (valueBuffer->data.values[0] > valueBuffer->data.values[1])
		Stop();

}

void Spark::Render()
{
	valueBuffer->SetVSBuffer(10);
	valueBuffer->SetPSBuffer(10);

	startColorBuffer->SetPSBuffer(11);
	endColorBuffer->SetPSBuffer(12);

	Particle::Render();
}

void Spark::Debug()
{
	ImGui::Text("Spark Option");

	ImGui::SliderInt("Particle Count", (int*)&drawCount, 1, MAX_COUNT);

	ImGui::ColorEdit4("StartColor", startColorBuffer->data.values);
	ImGui::ColorEdit4("EndColor", endColorBuffer->data.values);

}

void Spark::Play(Vector3 position)
{
	valueBuffer->data.values[0] = 0.f; // time

	Particle::Play(position);

	UpdateParticle();
	vertexBuffer->UpdateVertex(vertices.data(), drawCount);
}

void Spark::SetVertex()
{
	vertices.resize(MAX_COUNT); // VertexBuffer에서 data의 크기가 0으로 잡히지 않기 위함(ByteWidth)

	vertexBuffer = new VertexBuffer(vertices);

	drawCount = 500;

	valueBuffer->data.values[1] = 1.f;
}

void Spark::UpdateParticle()
{
	for (UINT i = 0; i < drawCount; i++)
	{
		vertices[i].position = this->position; // position - particle member var

		float size = Random(minSize, maxSize);
		vertices[i].size = { size, size };

		Vector3 rot{};

		rot.x = Random(0.f, XM_2PI);
		rot.y = Random(0.f, XM_2PI);
		rot.z = Random(0.f, XM_2PI);

		float radius = Random(minRadius, maxRadius);

		// 기본 velocity 방향에 rotMat을 곱해서 랜덤한 방면으로 보낼것임
		Vector3 velocity = Vector3(0, 0, radius);

		Matrix rotMatrix = XMMatrixRotationRollPitchYawFromVector(rot);

		vertices[i].velocity = XMVector3TransformCoord(velocity, rotMatrix);
	}
}
