#pragma once

/// <summary>
/// �׸����� �ػ󵵰� ���ƾ� ���ڰ� ����
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

	UINT width{}, height{}; // RenderTarget �ػ�

	RenderTarget* renderTarget{};
	DepthStencil* depthStencil{};

	ViewBuffer* viewBuffer{};
	MatrixBuffer* projBuffer{};

	Quad* quad{};

};
