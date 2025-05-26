#include "Framework.h"
#include "Water.h"


Water::Water(wstring normalFile, Vector2 size)
	:GameObject(L"28_Water"), size(size)
{
	name = "Water";

	reflection = new Reflection(this);
	refraction = new Refraction(normalFile);

	CreateMesh();
}

Water::~Water()
{
	delete refraction;
	delete reflection;
}

void Water::Update()
{
	reflection->Update();
	refraction->Update();

	GameObject::Update();
}

void Water::Render()
{
	reflection->SetRender();
	refraction->SetRender();

	GameObject::Render();
}

void Water::Debug()
{
	Transform::Debug();
	refraction->Debug();
}

void Water::SetReflection()
{
	reflection->SetPreRender();
}

void Water::SetRefraction()
{
	refraction->SetPreRender();
}

void Water::CreateMesh()
{
	float L = -size.x * 0.5f;
	float R = +size.x * 0.5f;
	float B = -size.y * 0.5f;
	float T = +size.y * 0.5f;

	vertices.emplace_back(Vector3(L, 0.f, T), Vector2(0, 0));
	vertices.emplace_back(Vector3(R, 0.f, T), Vector2(1, 0));
	vertices.emplace_back(Vector3(L, 0.f, B), Vector2(0, 1));
	vertices.emplace_back(Vector3(R, 0.f, B), Vector2(1, 1));

	indices =
	{
		0, 1, 2,
		2, 1, 3
	};

	mesh = new Mesh(vertices, indices);
}
