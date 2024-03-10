#pragma once


struct Ray
{
	Vector3 origin{};	 // ������
	Vector3 direction{}; // ����
};

struct HitResult // Ray�� �浹����
{
	Vector3 impactPoint{};
	float	distance{};

};


class Collider : public GameObject<Vertex>
{
public:
	Collider();
	virtual ~Collider();

	virtual void Render();

	bool Collision(Collider* other);

	virtual bool Collision(IN Ray ray, OUT HitResult* hitResult) = 0;
	virtual bool Collision(class ColliderSphere* other) = 0;
	virtual bool Collision(class ColliderBox*    other) = 0;
	virtual bool Collision(class ColliderCapsule* other) = 0;

	void SetColor(float r, float g, float b) { material->GetBuffer()->data.diffuse = { r, g, b, 1 }; }

private:

};
