#include "Framework.h"
#include "TerrainLOD.h"

TerrainLOD::TerrainLOD(wstring heightMap)
    :GameObject(L"31_TerrainLOD")
{
    material->SetDiffuseMap(L"Landscape/Dirt.png");

    hullShader      = Shader::AddHS(L"31_TerrainLOD");
    domainShader    = Shader::AddDS(L"31_TerrainLOD");

    this->heightMap = Texture::Add(heightMap);

    terrainBuffer = new FloatValueBuffer();
    heightBuffer = new FloatValueBuffer();

    terrainBuffer->data = { 1.0f, 1000.0f, 1.0f, 64.0f };

    heightBuffer->data.values[0] = 20.0f;

    CreateMesh();
}

TerrainLOD::~TerrainLOD()
{
    delete terrainBuffer;
    delete  heightBuffer;
}

void TerrainLOD::Render()
{
    terrainBuffer->SetHSBuffer(10);
    heightBuffer->SetDSBuffer(10);

    heightMap->DSSetShaderResources(10);

    hullShader->SetShader();
    domainShader->SetShader();

    GameObject::Render(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

    DC->HSSetShader(nullptr, nullptr, 0);
    DC->DSSetShader(nullptr, nullptr, 0);
}

void TerrainLOD::Debug()
{
    Transform::Debug();

    ImGui::Text("TerrainLOD Option");
    ImGui::DragFloat("MinDistance", &terrainBuffer->data.values[0]);
    ImGui::DragFloat("MaxDistance", &terrainBuffer->data.values[1]);
    ImGui::DragFloat("MinQuality", &terrainBuffer->data.values[2]);
    ImGui::DragFloat("MaxQuality", &terrainBuffer->data.values[3]);

    ImGui::DragFloat("HeightScale", &heightBuffer->data.values[0]);
}

void TerrainLOD::CreateMesh()
{
    width = heightMap->GetSize().x;
    height = heightMap->GetSize().y;

    cellUV = { 1.0f / width, 1.0f / height };

    patchWidth = ((width - 1) / cellsPerPatch) + 1;
    patchHeight = ((height - 1) / cellsPerPatch) + 1;

    vertices.resize(patchWidth * patchHeight);

    float halfWidth = width * cellSpacing * 0.5f;
    float halfHeight = height * cellSpacing * 0.5f;

    float tempWidth = width * cellSpacing / patchWidth;
    float tempHeight = height * cellSpacing / patchHeight;

    float du = 1.0f / patchWidth;
    float dv = 1.0f / patchHeight;

    for (UINT z = 0; z < patchHeight; z++)
    {
        float tempZ = halfHeight - z * tempHeight;

        for (UINT x = 0; x < patchWidth; x++)
        {
            float tempX = -halfHeight + x * tempHeight;

            UINT index = patchWidth * z + x;

            vertices[index].pos = { tempX, 0, tempZ };
            vertices[index].uv = { x * du, z * dv };
        }
    }

    indices.reserve((patchWidth - 1) * (patchHeight - 1) * 4);

    for (UINT z = 0; z < patchHeight - 1; z++)
    {
        for (UINT x = 0; x < patchWidth - 1; x++)
        {
            indices.push_back(x + 0 + (z + 0) * patchWidth);
            indices.push_back(x + 1 + (z + 0) * patchWidth);
            indices.push_back(x + 0 + (z + 1) * patchWidth);
            indices.push_back(x + 1 + (z + 1) * patchWidth);
        }
    }

    mesh = new Mesh(vertices, indices);
}