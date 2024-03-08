#include "Framework.h"
#include "Refraction.h"


UINT Refraction::index{};

Refraction::Refraction(wstring file)
{
	normalMap = Texture::Add(file);

	renderTarget = new RenderTarget(2048, 2048);
	depthStencil = new DepthStencil(2048, 2048);

	quad = new Quad(Vector2(200, 200));
	quad->translation = Vector3(100, 100, 0);

	Texture* texture = Texture::Add(L"Refraction_" + to_wstring(index++), renderTarget->GetSRV());

	quad->GetMaterial()->SetDiffuseMap(texture);
	quad->Update(); // 디버깅용이라 최초 업데이트 한번만 돌림

	buffer = new WaterBuffer;
}

Refraction::~Refraction()
{
	delete renderTarget;
	delete depthStencil;
	delete quad;

	delete buffer;
}

void Refraction::Update()
{
	buffer->data.waveTime += DELTA_TIME;
}

void Refraction::SetPreRender()
{
	renderTarget->Set(depthStencil);
}

void Refraction::SetRender()
{
	DC->PSSetShaderResources(11, 1, &renderTarget->GetSRV());
	normalMap->PSSetShaderResources(12);

	buffer->SetPSBuffer(10);
}

void Refraction::PostRender()
{
	quad->Render();
}

void Refraction::Debug()
{
	ImGui::Text("Water Option");
	ImGui::ColorEdit4("WaveColor", (float*)&buffer->data.color);

	ImGui::SliderFloat("WaveSpeed    ", (float*)&buffer->data.waveSpeed, 0.0f, 1.0f);
	ImGui::SliderFloat("WaveScale    ", (float*)&buffer->data.waveScale, 0.0f, 1.0f);
	ImGui::SliderFloat("WaveShininess", (float*)&buffer->data.waveShininess, 1.0f, 50.0f);
	ImGui::SliderFloat("WaveFresnel  ", (float*)&buffer->data.fresnel, 0.0f, 1.0f);
}
