#include "Framework.h"
#include "ColliderCapsule.h"

ColliderCapsule::ColliderCapsule(float radius, float height, UINT sliceCount, UINT stackCount)
    :radius(radius), height(height), sliceCount(sliceCount), stackCount(stackCount)
{
    CreateMesh();
}

ColliderCapsule::~ColliderCapsule()
{
}

bool ColliderCapsule::Collision(IN Ray ray, OUT HitResult* hitResult)
{
    Vector3 direction = up;
    Vector3 pa = GetGlobalPosition() - direction * Height() * 0.5f;
    Vector3 pb = GetGlobalPosition() + direction * Height() * 0.5f;

    float r = Radius();

    Vector3 ro = ray.origin;
    Vector3 rd = ray.direction;

    Vector3 ba = pb - pa;
    Vector3 oa = ro - pa;

    float baba = Vector3::Dot(ba, ba);
    float bard = Vector3::Dot(ba, rd);
    float baoa = Vector3::Dot(ba, oa);
    float rdoa = Vector3::Dot(rd, oa);
    float oaoa = Vector3::Dot(oa, oa);

    float a = baba - bard * bard;
    float b = baba * rdoa - baoa * bard;
    float c = baba * oaoa - baoa * baoa - r * r * baba;

    float h = b * b - a * c;

    if (h >= 0.0f)
    {
        float t = (-b - sqrt(h)) / a;

        float distance = baoa + t * bard;

        if (distance > 0.0f && distance < baba)
        {
            if (hitResult)
            {
                hitResult->distance = distance;
                hitResult->impactPoint = ray.origin + ray.direction * t;
            }

            return true;
        }

        Vector3 oc = (distance <= 0.0f) ? oa : ro - pb;
        b = Vector3::Dot(rd, oc);
        c = Vector3::Dot(oc, oc) - r * r;
        h = b * b - c;

        if (h > 0.0f)
        {
            if (hitResult)
            {
                hitResult->distance = distance;
                hitResult->impactPoint = ray.origin + ray.direction * t;
            }

            return true;
        }
    }

    return false;
}

bool ColliderCapsule::Collision(ColliderSphere* other)
{
    Vector3 direction = GetUpVector();
    Vector3 pa = GetGlobalPosition() - direction * Height() * 0.5f;
    Vector3 pb = GetGlobalPosition() + direction * Height() * 0.5f;

    Vector3 P = other->GetGlobalPosition();

    Vector3 pointOnLine = ClosestPointOnLine(pa, pb, P);

    float distance = (P - pointOnLine).Length();

    return distance <= (Radius() + other->Radius());
}

bool ColliderCapsule::Collision(ColliderBox* other)
{
    ColliderBox::OBB box = other->GetOBB();

    Vector3 direction = GetUpVector();
    Vector3 pa = globalPosition - direction * Height() * 0.5f;
    Vector3 pb = globalPosition + direction * Height() * 0.5f;

    Vector3 closestPointToSphere = other->GetGlobalPosition();
    Vector3 pointOnLine          = ClosestPointOnLine(pa, pb, other->GetGlobalPosition());
    Vector3 boxToSphere          = pointOnLine - other->GetGlobalPosition();

    for (UINT i = 0; i < 3; i++)
    {
        float length    = Vector3::Dot(box.axis[i], boxToSphere);
        float mult      = (length < 0.0f) ? -1.0f : 1.0f;
        length          = min(abs(length), box.halfSize[i]);
        closestPointToSphere += box.axis[i] * length * mult;
    }

    float distance = (pointOnLine - closestPointToSphere).Length();

    return distance <= Radius();
}

bool ColliderCapsule::Collision(ColliderCapsule* other)
{
    Vector3 aDirection = GetUpVector();
    Vector3 aA = GetGlobalPosition() - aDirection * Height() * 0.5f;
    Vector3 aB = GetGlobalPosition() + aDirection * Height() * 0.5f;

    Vector3 bDirection = other->GetUpVector();
    Vector3 bA = other->GetGlobalPosition() - bDirection * other->Height() * 0.5f;
    Vector3 bB = other->GetGlobalPosition() + bDirection * other->Height() * 0.5f;

    Vector3 v0 = bA - aA;
    Vector3 v1 = bB - aA;
    Vector3 v2 = bA - aB;
    Vector3 v3 = bB - aB;

    float d0 = Vector3::Dot(v0, v0);
    float d1 = Vector3::Dot(v1, v1);
    float d2 = Vector3::Dot(v2, v2);
    float d3 = Vector3::Dot(v3, v3);

    Vector3 bestA;
    if (d2 < d0 || d2 < d1 || d3 < d0 || d3 > d1)
        bestA = aB;
    else
        bestA = aA;

    Vector3 bestB = ClosestPointOnLine(bA, bB, bestA);
    bestA = ClosestPointOnLine(aA, aB, bestB);
    bestB = ClosestPointOnLine(bA, bB, bestA);

    float distance = (bestA - bestB).Length();
    float temp = (Radius() + other->Radius());
    return distance <= (Radius() + other->Radius());
}

void ColliderCapsule::CreateMesh()
{
    float phiStep	= XM_PI / stackCount;
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
			
			if (vertex.pos.y > 0)		vertex.pos.y += height * 0.5f;
			else if (vertex.pos.y < 0)	vertex.pos.y -= height * 0.5f;
			
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
