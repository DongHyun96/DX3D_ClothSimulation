#include "Framework.h"
#include "Collider.h"


Collider::Collider()
	:GameObject(L"10_Collider")
{
	SetColor(0, 1, 0);
}

Collider::~Collider()
{
}

void Collider::Render()
{
	if (!isActive) return;

	GameObject::Render(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

bool Collider::Collision(Collider* other)
{
	if (!this->isActive)  return false;
	if (!other->isActive) return false;

	ColliderBox* box = dynamic_cast<ColliderBox*>(other);
	if (box) return Collision(box);

	ColliderSphere* sphere = dynamic_cast<ColliderSphere*>(other);
	if (sphere) return Collision(sphere);

	ColliderCapsule* capsule = dynamic_cast<ColliderCapsule*>(other);
	if (capsule) return Collision(capsule);

	return false;
}

