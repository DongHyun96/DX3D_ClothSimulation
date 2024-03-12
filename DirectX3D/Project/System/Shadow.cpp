#include "Framework.h"
#include "Shadow.h"


Shadow::Shadow(UINT width, UINT height)
	:width(width), height(height)
{
	static int index = 0;

	renderTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);

	quad = new Quad(Vector2(200, 200));
	quad->translation = Vector3(100, 100, 0);

	Texture* texture = Texture::Add(L"Shadow_" + to_wstring(index++), renderTarget->GetSRV());

	quad->GetMaterial()->SetDiffuseMap(texture);
	quad->Update(); // 디버깅용이라 최초 업데이트 한번만 돌림

	viewBuffer = new ViewBuffer;
	projBuffer = new MatrixBuffer;
}

Shadow::~Shadow()
{
	delete renderTarget;
	delete depthStencil;
	delete viewBuffer;
	delete projBuffer;
	delete quad;
}

void Shadow::SetPreRender()
{
	renderTarget->Set(depthStencil);

	SetViewProjection();

}

void Shadow::SetRender()
{
	viewBuffer->SetVSBuffer(11);
	projBuffer->SetVSBuffer(12);

	DC->PSSetShaderResources(10, 1, &renderTarget->GetSRV());
}

void Shadow::PostRender()
{
	quad->Render();
}

void Shadow::SetViewProjection()
{
	// 0번 light 하나만 shadow 계산
	LightData& data = ENVIRONMENT->GetLightBuffer()->data.lights[0];

	Matrix view = XMMatrixLookAtLH(data.position, Vector3(0, 0, 0), Vector3(0, 1, 0));

	Matrix proj = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.f, 0.1f, 10000.f);

	viewBuffer->SetData(view, XMMatrixInverse(nullptr, view));
	projBuffer->SetData(proj);

	viewBuffer->SetVSBuffer(1);
	projBuffer->SetVSBuffer(2);
}
