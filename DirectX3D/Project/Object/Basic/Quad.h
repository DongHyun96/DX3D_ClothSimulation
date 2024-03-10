#pragma once

class Quad : public GameObject<VertexTextureNormalTangent>
{
	typedef VertexTextureNormalTangent VertexType;

public:
	Quad(Vector2 size = { 1, 1 });
	~Quad();

	void Render();

	Vector3 GetNormal() const { return forward * (-1); }

private:
	void CreateMesh() override;

	void CalculateTangent();

private:
	Vector2 size{};
};