#pragma once

/// <summary>
/// 그림자의 해상도가 높아야 예쁘게 나옴
/// </summary>
class Shadow
{
public:
	Shadow(UINT width = 2048, UINT height = 2048);
	~Shadow();

	void SetPreRender();
	void SetRender();

	void PostRender();

private:

	void SetViewProjection();

private:

	UINT width{}, height{}; // RenderTarget 해상도

	RenderTarget* renderTarget{};
	DepthStencil* depthStencil{};

	ViewBuffer* viewBuffer{};
	MatrixBuffer* projBuffer{};

	Quad* quad{};

};
