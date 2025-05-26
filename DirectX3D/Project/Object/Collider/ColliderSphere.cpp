#include "Framework.h"
#include "ColliderSphere.h"


ColliderSphere::ColliderSphere(float radius, UINT stackCount, UINT sliceCount)
	:radius(radius), stackCount(stackCount), sliceCount(sliceCount)
{
	CreateMesh();
}

ColliderSphere::~ColliderSphere()
{
}

void ColliderSphere::CreateMesh()
{
	float phiStep = XM_PI / stackCount;
	float thetaStep = XM_2PI / sliceCount;

	for (UINT i = 0; i < stackCount + 1; i++)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j < sliceCount + 1; j++)
		{
			float theta = j * thetaStep;

			Vertex vertex(0, 0, 0);

			vertex.pos.x = radius * sin(phi) * cos(theta);
			vertex.pos.y = radius * cos(phi);
			vertex.pos.z = radius * sin(phi) * sin(theta);

			vertices.emplace_back(vertex);
		}
	}

	for (UINT i = 0; i < stackCount; i++)
	{
		for (UINT j = 0; j < sliceCount; j++)
		{
			indices.emplace_back((i + 0) * (sliceCount + 1) + (j + 0));
			indices.emplace_back((i + 1) * (sliceCount + 1) + (j + 0));

			indices.emplace_back((i + 0) * (sliceCount + 1) + (j + 0));
			indices.emplace_back((i + 0) * (sliceCount + 1) + (j + 1));
		}
	}

	mesh = new Mesh(vertices, indices);
}

bool ColliderSphere::Collision(IN Ray ray, OUT HitResult* hitResult)
{
	Vector3 O = ray.origin;
	Vector3 D = ray.direction;

	Vector3 P = this->globalPosition;
	Vector3 X = O - P;

	// 자기자신 Dot product = 길이의 제곱
	float a = Vector3::Dot(D, D);
	float b = 2 * Vector3::Dot(D, X);
	float c = Vector3::Dot(X, X) - Radius() * Radius();

	if (b * b - 4 * a * c >= 0)
	{
		if (hitResult)
		{
			float t = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

			hitResult->distance = t;
			hitResult->impactPoint = O + D * t;
		}

		return true;
	}

	return false;
}

bool ColliderSphere::Collision(ColliderSphere* other)
{
	float distance = (this->globalPosition - other->globalPosition).Length();

	return distance < this->Radius() + other->Radius();
}

bool ColliderSphere::Collision(ColliderBox* other)
{
	return other->Collision(this);
}

bool ColliderSphere::Collision(ColliderCapsule* other)
{
	return other->Collision(this);
}
