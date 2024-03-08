#include "Framework.h"
#include "Reflection.h"

UINT Reflection::index{};

Reflection::Reflection(Transform* target)
	:target(target)
{
	camera = new Camera;

	renderTarget = new RenderTarget(2048, 2048);
	depthStencil = new DepthStencil(2048, 2048);

	quad = new Quad(Vector2(200, 200));
	quad->translation = Vector3(100, 100, 0);

	Texture* texture = Texture::Add(L"Reflection_" + to_wstring(index++), renderTarget->GetSRV());

	quad->GetMaterial()->SetDiffuseMap(texture);
	quad->Update(); // 디버깅용이라 최초 업데이트 한번만 돌림
}

Reflection::~Reflection()
{
	delete renderTarget;
	delete depthStencil;
	delete camera;
	delete quad;
}

void Reflection::Update()
{
	Vector3 pos = CAMERA->translation;
	Vector3 rot = CAMERA->rotation;

	pos.y = target->GetGlobalPosition().y * 2.f - pos.y;
	rot.x *= -1;

	camera->translation = pos;
	camera->rotation = rot;

	camera->UpdateWorldMatrix();
}

void Reflection::SetPreRender()
{
	renderTarget->Set(depthStencil);

	camera->SetView();
}

void Reflection::SetRender()
{
	// 반사용 카메라를 따로 view Mat을 세팅해서 넘겨줌
	camera->GetViewBuffer()->SetVSBuffer(10);

	DC->PSSetShaderResources(10, 1, &renderTarget->GetSRV());
}

void Reflection::PostRender()
{
	quad->Render();
}
