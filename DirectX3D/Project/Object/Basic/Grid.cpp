#include "Framework.h"
#include "Grid.h"


Grid::Grid()
	:GameObject(L"Tutorial")
{
	CreateMesh();
}

Grid::~Grid()
{
}

void Grid::CreateMesh()
{
	vertices.emplace_back(Vector3(-size.x, 0, 0), Vector4(1, 0, 0, 1));
	vertices.emplace_back(Vector3(+size.x, 0, 0), Vector4(1, 0, 0, 1));

	vertices.emplace_back(Vector3(0, -size.x, 0), Vector4(0, 1, 0, 1));
	vertices.emplace_back(Vector3(0, +size.x, 0), Vector4(0, 1, 0, 1));

	vertices.emplace_back(Vector3(0, 0, -size.x), Vector4(0, 0, 1, 1));
	vertices.emplace_back(Vector3(0, 0, +size.x), Vector4(0, 0, 1, 1));

	for (int i = -size.x; i < size.x + 1; i++)
	{
		if (i == 0) continue;

		vertices.emplace_back(Vector3(i, 0, -size.y), Vector4(0.5f, 0.5f, 0.5f, 1.f));
		vertices.emplace_back(Vector3(i, 0, +size.y), Vector4(0.5f, 0.5f, 0.5f, 1.f));
	}

	for (int i = -size.y; i < size.y + 1; i++)
	{
		if (i == 0) continue;

		vertices.emplace_back(Vector3(-size.x, 0, i), Vector4(0.5f, 0.5f, 0.5f, 1.f));
		vertices.emplace_back(Vector3(+size.x, 0, i), Vector4(0.5f, 0.5f, 0.5f, 1.f));
	}

	mesh = new Mesh(vertices);

}
