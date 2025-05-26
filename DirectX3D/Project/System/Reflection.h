#pragma once
class Reflection
{
public:
	Reflection(Transform* target); // 어떤 걸 반사시킬지 지정
	~Reflection();

	void Update();

	void SetPreRender();
	void SetRender();

	void PostRender();

private:

	Transform* target{};

	RenderTarget* renderTarget{};
	DepthStencil* depthStencil{};

	Camera* camera{};

	Quad* quad{};

	static UINT index;

};
