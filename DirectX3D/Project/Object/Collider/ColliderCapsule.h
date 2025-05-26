#pragma once

class ColliderCapsule : public Collider
{
public:
	ColliderCapsule(float radius = 1, float height = 2.f, UINT sliceCount = 8, UINT stackCount = 16);
	~ColliderCapsule();

	bool Collision(IN Ray ray, OUT HitResult* hitResult) override;
	bool Collision(ColliderSphere* other)				 override;
	bool Collision(ColliderBox* other)					 override;
	bool Collision(ColliderCapsule* other)				 override;

	float Radius() const { return radius * Max(globalScale.x, globalScale.y, globalScale.z); }

	float Height() const { return height * globalScale.y; }

private:

	void CreateMesh() override;



private:

	float radius{}, height{};

	UINT stackCount{}, sliceCount{};

};
