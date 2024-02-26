#pragma once
class RenderTarget
{
public:
	RenderTarget(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
	~RenderTarget();

	void Set(DepthStencil* depthStencil, Vector4 clearColor = Vector4(1, 1, 1, 1));

	ID3D11RenderTargetView  *& GetRTV() { return rtv;}
	ID3D11ShaderResourceView*& GetSRV() { return srv; }

	static void SetMultiRenderTarget(RenderTarget** targets, UINT count, DepthStencil* depthStencil, Vector4 clearColor = Vector4(1,1,1,1));

private:

	void CreateTexture();
	void CreateRTV();
	void CreateSRV();

private:

	UINT width, height;

	ID3D11Texture2D* texture{};

	ID3D11RenderTargetView*   rtv{};
	ID3D11ShaderResourceView* srv{};
};
