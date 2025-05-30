﻿#include "Framework.h"
#include "SamplerState.h"


SamplerState::SamplerState()
{
	D3D11_SAMPLER_DESC desc{};
	desc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias		= 0;
	desc.MaxAnisotropy	= 0;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD			= 0;
	desc.MaxLOD			= D3D11_FLOAT32_MAX;

	DEVICE->CreateSamplerState(&desc, &samplerState);

	DC->PSSetSamplers(0, 1, &samplerState);
}

SamplerState::~SamplerState()
{
	samplerState->Release();
}
