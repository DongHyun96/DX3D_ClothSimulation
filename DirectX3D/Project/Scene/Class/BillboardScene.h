#pragma once
class BillboardScene : public Scene
{
public:
	BillboardScene();
	~BillboardScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:
	Terrain* terrain{};

	//vector<Quad*> trees{};

	vector<VertexTexture> vertices{};
	vector<VertexTexture> drawVertices{};


	Material* material{};
	VertexBuffer* vertexBuffer{};
	GeometryShader* geometryShader{};

	const UINT MAX_COUNT = 10000;

	UINT drawCount = MAX_COUNT;

};
