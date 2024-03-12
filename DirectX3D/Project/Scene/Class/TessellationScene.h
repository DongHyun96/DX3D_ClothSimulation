#pragma once

class TessellationScene : public Scene
{
public:
	TessellationScene();
	~TessellationScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:
	Material* material{};

	HullShader* hullShader{};
	DomainShader* domainShader{};

	VertexBuffer* vertexBuffer{};

	vector<Vertex>		vertices{};

	FloatValueBuffer* edgeBuffer{};
	FloatValueBuffer* insideBuffer{};

};