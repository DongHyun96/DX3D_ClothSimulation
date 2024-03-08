#include "Framework.h"
#include "Rain.h"

Rain::Rain(wstring file)
{
    material->SetShader(L"24_Rain");
    material->SetDiffuseMap(file);

    geometryShader = Shader::AddGS(L"24_Rain");

    buffer = new WeatherBuffer();

    SetVertex();
}

Rain::~Rain()
{
    delete buffer;
}

void Rain::Update()
{
    buffer->data.time += Time::Delta();
}

void Rain::Render()
{
    buffer->SetVSBuffer(10);

    Particle::Render();
}

void Rain::Debug()
{
    ImGui::Text("Rain Option");
    ImGui::DragFloat3("Velocity",   (float*)&buffer->data.velocity, 0.1f);
    ImGui::DragFloat("Distance",    (float*)&buffer->data.distance);
    ImGui::DragFloat("Turbulence",  (float*)&buffer->data.turbulence, 0.1f, 0.0f, 10.0f);
    ImGui::DragFloat3("Origin",     (float*)&buffer->data.origin, 0.1f);
    ImGui::DragFloat3("Size",       (float*)&buffer->data.size, 0.1f);
    ImGui::ColorEdit3("Color",      (float*)&buffer->data.color);
}

void Rain::SetVertex()
{
    drawCount = MAX_COUNT;

    vertices.resize(drawCount);

    for (UINT i = 0; i < drawCount; i++)
    {
        Vector2 size;

        size.x = Random(0.1f, 0.4f);
        size.y = Random(4.0f, 8.0f);

        vertices[i].uv = size;

        Vector3 pos;

        pos.x = Random(-buffer->data.size.x, +buffer->data.size.x);
        pos.y = Random(-buffer->data.size.y, +buffer->data.size.y);
        pos.z = Random(-buffer->data.size.z, +buffer->data.size.z);

        vertices[i].pos = pos;
    }

    vertexBuffer = new VertexBuffer(vertices);
}