#include "Framework.h"
#include "Sphere.h"


Sphere::Sphere(float radius, UINT sliceCount, UINT stackCount)
	:GameObject(L"05_NormalMapping"), radius(radius), sliceCount(sliceCount), stackCount(stackCount)
{
	CreateMesh();
}

Sphere::~Sphere()
{
}

void Sphere::CreateMesh()
{
	float phiStep	= XM_PI / stackCount;
	float thetaStep = XM_2PI / sliceCount;

	for (UINT i = 0; i < stackCount + 1; i++)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j < sliceCount + 1; j++)
		{
			float theta = j * thetaStep;

			VertexType vertex;

			vertex.pos.x = radius * sin(phi) * cos(theta);
			vertex.pos.y = radius * cos(phi);
			vertex.pos.z = radius * sin(phi) * sin(theta);

			vertex.uv.x = j / (float)sliceCount;
			vertex.uv.y = i / (float)stackCount;

			vertex.normal = vertex.pos / radius;

			vertices.emplace_back(vertex);
		}
	}

	for (UINT i = 0; i < stackCount; i++)
	{
		for (UINT j = 0; j < sliceCount; j++)
		{
			indices.emplace_back((i + 0) * (sliceCount + 1) + (j + 0));
			indices.emplace_back((i + 0) * (sliceCount + 1) + (j + 1));
			indices.emplace_back((i + 1) * (sliceCount + 1) + (j + 0));

			indices.emplace_back((i + 1) * (sliceCount + 1) + (j + 0));
			indices.emplace_back((i + 0) * (sliceCount + 1) + (j + 1));
			indices.emplace_back((i + 1) * (sliceCount + 1) + (j + 1));
		}
	}

	CalculateTangent();

	mesh = new Mesh(vertices, indices);
}

void Sphere::CalculateTangent()
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

