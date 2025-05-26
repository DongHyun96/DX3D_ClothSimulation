#pragma once
class GeometryBuffer
{
	enum RenderTargets
	{
		DIFFUSE, NORMAL, SPECULAR, SIZE
	};

public:
	GeometryBuffer();
	~GeometryBuffer();

	void SetMultiRenderTarget();
	void PostRender();
	void SetSRVs();

private:
	RenderTarget* rtvs[SIZE];

	DepthStencil* depthStencil{};

	Quad* quads[SIZE + 1]; // 디버깅용 quad들

	ID3D11ShaderResourceView* srvs[SIZE + 1];
};
