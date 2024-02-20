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
	
	void Render(D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) override;


private:
	
	void CreateMesh() override;

	void CalculateTangent();

private:

	UINT width{ 10 }, height{ 10 };
	
	Texture* heightMap{};

	// heightMap �� �ִ���̴� 1, scale�� ���ؼ� �� Ű��
	const float HEIGHT_SCALE = 20.f;

	Texture* alphaMap{};
	Texture* secondDiffuseMap{};
};
