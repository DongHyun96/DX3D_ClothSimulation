#pragma once

//struct OBBQuad
//{
//	Vector3 axis[3]{};
//	Vector3 halfSize{};
//};

struct OBBQuad
{
	Vector3 axis[2]{};
	Vector3 halfSize{};
};

class Quad : public GameObject<VertexTextureNormalTangent>
{
	typedef VertexTextureNormalTangent VertexType;

public:
	Quad(Vector2 size = { 1, 1 });
	~Quad();

	void Render();

	Vector3 GetNormal() const { return forward * (-1); }

	OBBQuad GetOBB() const;

private:
	void CreateMesh() override;

	void CalculateTangent();

private:
	Vector2 size{};
};