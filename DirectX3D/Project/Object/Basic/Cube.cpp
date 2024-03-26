#include "Framework.h"
#include "Cube.h"


Cube::Cube(Vector4 color)
    :GameObject<VertexType>(L"02_DiffuseColor"), color(color)
{
    CreateMesh();
}

Cube::~Cube()
{
}

void Cube::Render()
{
    GameObject::Render();
}

void Cube::CreateMesh()
{
    // Create Vertex
   // Back-Face Culling (cull 없애다, 절단하다, 도태시키다)
    vertices =
    {
        {XMFLOAT3(-0.5f, +0.5f, -0.5f), color},
        {XMFLOAT3(+0.5f, +0.5f, -0.5f), color},
        {XMFLOAT3(-0.5f, -0.5f, -0.5f), color},
        {XMFLOAT3(+0.5f, -0.5f, -0.5f), color},

        {XMFLOAT3(-0.5f, +0.5f, +0.5f), color},
        {XMFLOAT3(+0.5f, +0.5f, +0.5f), color},
        {XMFLOAT3(-0.5f, -0.5f, +0.5f), color},
        {XMFLOAT3(+0.5f, -0.5f, +0.5f), color}
    };

    indices =
    {
        // Front
        0, 1, 2,
        2, 1, 3,

        // Back
        5, 4, 7,
        7, 4, 6,

        // Left
        4, 0, 6,
        6, 0, 2,

        // Right
        1, 5, 3,
        3, 5, 7,

        // Top
        0, 4, 1,
        1, 4, 5,

        // Bottom
        2, 3, 6,
        6, 3, 7
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

    mesh = new Mesh(vertices, indices);
}

