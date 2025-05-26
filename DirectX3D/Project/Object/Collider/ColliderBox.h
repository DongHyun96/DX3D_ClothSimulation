#pragma once

class ColliderBox : public Collider
{
public:
	struct OBB
	{
		Vector3 axis[3]{}; // local x,y,z축
		Vector3 halfSize{};
	};

public:
	ColliderBox(Vector3 size = {1, 1, 1});
	~ColliderBox();

	bool Collision(IN Ray ray, OUT HitResult* hitResult) override;
	bool Collision(ColliderSphere* other)				 override;
	bool Collision(ColliderBox* other)					 override;
	bool Collision(ColliderCapsule* other)				 override;

	OBB GetOBB();

	// distance - 중점끼리 뺀 것
	bool SeperatedAxis(Vector3 distance, Vector3 axis, OBB box1, OBB box2);

private:
	void CreateMesh() override;

private:
	Vector3 size{};


};
