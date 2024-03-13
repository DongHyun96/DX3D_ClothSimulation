#pragma once
class TerrainLOD : public GameObject<VertexTexture>
{
private:
	typedef VertexTexture VertexType;

public:
	TerrainLOD(wstring heightMap);
	~TerrainLOD();

	void Render();
	void Debug();

private:

	void CreateMesh() override;

private:

	UINT      width{}, height{};
	UINT patchWidth{}, patchHeight{};

	UINT				cellsPerPatch = 32;
	float				cellSpacing = 5.f;
	Vector2				cellUV{};

	Texture* heightMap{};

	FloatValueBuffer* terrainBuffer{};
	FloatValueBuffer* heightBuffer{};

	HullShader* hullShader{};
	DomainShader* domainShader{};

};
