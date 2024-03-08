#include "Framework.h"
#include "RenderTarget.h"


RenderTarget::RenderTarget(UINT width, UINT height)
	:width(width), height(height)
{
	CreateTexture();
	CreateRTV();
	CreateSRV();
}

RenderTarget::~RenderTarget()
{
	texture->Release();
	rtv->Release();
	srv->Release();
}

void RenderTarget::Set(DepthStencil* depthStencil, Vector4 clearColor)
{
	DC->OMSetRenderTargets(1, &rtv, depthStencil->GetDSV());

	DC->ClearRenderTargetView(rtv, (float*)&clearColor);

	depthStencil->ClearDSV();

	// Viewport도 새로 잡아줘야 함
	Device::GetInstance()->SetViewport(width, height);

	ENVIRONMENT->Set();
}

void RenderTarget::SetMultiRenderTarget(RenderTarget** targets, UINT count, DepthStencil* depthStencil, Vector4 clearColor)
{

	vector<ID3D11RenderTargetView*> rtvs{};

	for (UINT i = 0; i < count; i++)
	{
		rtvs.emplace_back(targets[i]->GetRTV());
		DC->ClearRenderTargetView(rtvs.back(), (float*)&clearColor);
	}

	depthStencil->ClearDSV();

	DC->OMSetRenderTargets(count, rtvs.data(), depthStencil->GetDSV());

	//Device::GetInstance()->SetViewport(width, height);
	ENVIRONMENT->Set();
}

void RenderTarget::CreateTexture()
{
	D3D11_TEXTURE2D_DESC desc{};

    desc.Width                 = width;
    desc.Height                = height;
    desc.MipLevels             = 1;
    desc.ArraySize             = 1;
    desc.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count      = 1;
    desc.SampleDesc.Quality    = 0;
    desc.Usage                 = D3D11_USAGE_DEFAULT;
    desc.BindFlags             = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags        = 0;
    desc.MiscFlags             = 0;

    DEVICE->CreateTexture2D(&desc, nullptr, &texture);
}

void RenderTarget::CreateRTV()
{
	D3D11_RENDER_TARGET_VIEW_DESC desc{};

	desc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension	= D3D11_RTV_DIMENSION_TEXTURE2D;

	DEVICE->CreateRenderTargetView(texture, &desc, &rtv);
}

void RenderTarget::CreateSRV()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc{};

    desc.Format                     = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.ViewDimension              = D3D11_SRV_DIMENSION_TEXTURE2D;
    desc.Texture2DArray.MipLevels   = 1;

    DEVICE->CreateShaderResourceView(texture, &desc, &srv);
}

