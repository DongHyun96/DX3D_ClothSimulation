#pragma once

struct Vertex
{
    Vertex(XMFLOAT3 pos)
        :pos(pos) {}

    Vertex(float x, float y, float z)
    {
        pos = { x, y, z };
    }

    XMFLOAT3 pos{};
};

struct VertexColor
{
    VertexColor(XMFLOAT3 pos, XMFLOAT4 color)
        :pos(pos), color(color) {}

    XMFLOAT3 pos{};
    XMFLOAT4 color{};
};

struct VertexTexture
{
    VertexTexture() {}

    VertexTexture(Vector3 pos, Vector2 uv)
    :pos(pos), uv(uv) {}

    Vector3 pos{};
    Vector2 uv{};
};

struct VertexTextureNormal
{
    VertexTextureNormal() {}

    VertexTextureNormal(Vector3 pos, Vector2 uv)
        :pos(pos), uv(uv) {}

    Vector3 pos{};
    Vector2 uv{};
    Vector3 normal{};
};

struct VertexColorNormal
{
    VertexColorNormal() {}

    VertexColorNormal(XMFLOAT3 pos, XMFLOAT4 color)
        :pos(pos), color(color) {}

    Vector3 pos{};
    Vector4 color{};
    Vector3 normal{};
};

struct VertexTextureNormalTangent
{
    VertexTextureNormalTangent() {}

    VertexTextureNormalTangent(Vector3 pos, Vector2 uv)
        :pos(pos), uv(uv) {}

    Vector3 pos{};
    Vector2 uv{};
    Vector3 normal{};
    Vector3 tangent{};
};

struct VertexTextureNormalTangentBlend
{
    Vector3 pos{};
    Vector2 uv{};
    Vector3 normal{};
    Vector3 tangent{};

    // Bone 관련 데이터
    int     indices[4]{};
    float   weights[4]{};
    
};