#include "Framework.h"
#include "Quad.h"

Quad::Quad(Vector2 size)
	:GameObject<VertexType>(L"01_Texture"), size(size)
{
	CreateMesh();
}

Quad::~Quad()
{
}

void Quad::Render()
{
	GameObject::Render();
}

OBBQuad Quad::GetOBB() const
{
	/*OBBQuad obbData{};

	Vector3 halfSize =
	{
		size.x * 0.5f * globalScale.x,
		0.f,
		size.y * 0.5f * globalScale.z
	};

	obbData.axis = this->GetRightVector();

	return obbData;*/

	OBBQuad obbData{};

	Vector3 halfSize =
	{
		size.x * 0.5f * globalScale.x,
		size.y * 0.5f * globalScale.y,
		0.f
	};

	obbData.axis[0] = this->GetRightVector();
	obbData.axis[1] = this->GetUpVector();
	//obbData.axis[2] = this->GetForwardVector();

	return obbData;
}

void Quad::CreateMesh()
{
	float L = -size.x * 0.5f;
	float R = +size.x * 0.5f;
	float B = -size.y * 0.5f;
	float T = +size.y * 0.5f;

	vertices.emplace_back(Vector3(L, T, 0.f), Vector2(0, 0));
	vertices.emplace_back(Vector3(R, T, 0.f), Vector2(1, 0));
	vertices.emplace_back(Vector3(L, B, 0.f), Vector2(0, 1));
	vertices.emplace_back(Vector3(R, B, 0.f), Vector2(1, 1));

	indices =
	{
		0, 1, 2,
		2, 1, 3
	};

	// Create Normal Vector
	for (UINT i = 0; i < indices.size() / 3; i++) // 폴리곤의 개수만큼 normal 생성
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v01 = vertices[index1].pos - vertices[index0].pos;
		Vector3 v02 = vertices[index2].pos - vertices[index0].pos;

		Vector3 normal = Vector3::Cross(v01, v02).GetNormalized();

		// 추후 평균을 냄
		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
	}

	CalculateTangent();

	mesh = new Mesh(vertices, indices);

}

void Quad::CalculateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 p0 = vertices[index0].pos;
		Vector3 p1 = vertices[index1].pos;
		Vector3 p2 = vertices[index2].pos;

		Vector2 uv0 = vertices[index0].uv;
		Vector2 uv1 = vertices[index1].uv;
		Vector2 uv2 = vertices[index2].uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float v0 = uv1.y - uv0.y;

		float u1 = uv2.x - uv0.x;
		float v1 = uv2.y - uv0.y;

		float D = 1.f / (u0 * v1 - v0 * u1);

		Vector3 tangent = D * (e0 * v1 - e1 * v0);

		vertices[index0].tangent += tangent;
		vertices[index1].tangent += tangent;
		vertices[index2].tangent += tangent;
	}

	// Tangent길이 보정
	for (VertexType& vertex : vertices)
	{
		Vector3 T = vertex.tangent;
		Vector3 N = vertex.normal;

		vertex.tangent = Vector3(T - N * Vector3::Dot(N, T)).GetNormalized();
	}
}


