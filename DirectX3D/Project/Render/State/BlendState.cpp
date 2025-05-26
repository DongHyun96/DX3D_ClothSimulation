#include "Framework.h"
#include "BlendState.h"


BlendState::BlendState()
{
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable			= false;
	desc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	DEVICE->CreateBlendState(&desc, &state);
}

BlendState::~BlendState()
{
	state->Release();
}

void BlendState::SetState()
{
	// Sample Mask -> 0xff ff ff ff / RGBA 각 값의 sample을 허용하는지 안하는지 false, true (0, 1)
	// 그냥 다 허용한다고 보면 됨
	DC->OMSetBlendState(state, nullptr, 0xFFFFFFFF);
}

void BlendState::CreateState()
{
	if (state) state->Release();

	DEVICE->CreateBlendState(&desc, &state);
}

void BlendState::SetAlpha(bool isAlpha)
{
	desc.RenderTarget[0].BlendEnable = isAlpha;

	CreateState();
}

void BlendState::SetAlphaToCoverage(bool isAlphaToCoverage)
{
	desc.AlphaToCoverageEnable = isAlphaToCoverage;

	CreateState();
}

void BlendState::SetAdditive()
{
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	CreateState();

}
