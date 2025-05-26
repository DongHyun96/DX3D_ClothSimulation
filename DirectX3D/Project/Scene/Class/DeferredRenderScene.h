#pragma once
class DeferredRenderScene : public Scene
{
public:
	DeferredRenderScene();
	~DeferredRenderScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

	void CreateObjects();

private:

	
	Quad* floor{};

	ModelAnimator* knight{};

	Sphere* sphere{};

	GeometryBuffer* gBuffer{};

	Material*		material{};
	VertexBuffer*	vertexBuffer{};

};
