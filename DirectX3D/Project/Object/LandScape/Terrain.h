#pragma once
class Terrain : public GameObject<VertexTextureNormalTangent>
{

	typedef VertexTextureNormalTangent VertexType;

public:
	Terrain(wstring heightMapFile);
	Terrain(wstring heightMapFile, wstring alphaMapFile);

	~Terrain();

	float GetHeight(Vector3 position);

	void SetSecondDiffuseMap(wstring file) { secondDiffuseMap = Texture::Add(file); }
	
	virtual void Render(D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) override;

	Vector2 GetSize() const { return Vector2(width, height); }


private:
	
	void CreateMesh() override;

	void CalculateTangent();

private:

	UINT width{ 10 }, height{ 10 };
	
	Texture* heightMap{};

	// heightMap 의 최대높이는 1, scale을 곱해서 더 키움
	const float HEIGHT_SCALE = 20.f;

	Texture* alphaMap{};
	Texture* secondDiffuseMap{};
};
