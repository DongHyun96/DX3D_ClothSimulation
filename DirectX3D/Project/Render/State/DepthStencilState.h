﻿#pragma once
class DepthStencilState
{
public:
	DepthStencilState();
	~DepthStencilState();

	void SetState(UINT stencilRef = 1);

	void ChangeState();

	void DepthEnable(bool value);

	void DepthWriteMask(D3D11_DEPTH_WRITE_MASK value);

private:

	D3D11_DEPTH_STENCIL_DESC desc{};
	ID3D11DepthStencilState* state{};

};
