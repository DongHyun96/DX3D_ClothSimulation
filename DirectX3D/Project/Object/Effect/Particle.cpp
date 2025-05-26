#include "Framework.h"
#include "Particle.h"

Particle::Particle()
{
	material = new Material;
}

Particle::~Particle()
{
	delete material;
	delete vertexBuffer;
}

void Particle::Render()
{
	if (!isActive) return;

	if (isAdditive) STATE->EnableAdditive();
	else			STATE->EnableAlpha();


	STATE->DepthWriteMaskZero();

	vertexBuffer->IASetBuffer(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	material->Set();
	geometryShader->SetShader();

	DC->Draw(drawCount, 0);

	DC->GSSetShader(nullptr, nullptr, 0);

	STATE->DisableAlpha();
	STATE->EnableDepth();
}

void Particle::Play(Vector3 position)
{
	isActive = true;

	this->position = position;
}

void Particle::Stop()
{
	isActive = false;
}