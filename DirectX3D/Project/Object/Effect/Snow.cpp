#include "Framework.h"
#include "Snow.h"

Snow::Snow(wstring file)
{
    material->SetShader(L"25_Snow");
    material->SetDiffuseMap(file);

    geometryShader = Shader::AddGS(L"25_Snow");

    buffer = new WeatherBuffer();

    SetVertex();
}

Snow::~Snow()
{
    delete buffer;
}

void Snow::Update()
{
    buffer->data.time += Time::Delta();
}

void Snow::Render()
{
    buffer->SetVSBuffer(10);

    Particle::Render();
}

void Snow::Debug()
{
    ImGui::Text("Snow Option");
    ImGui::DragFloat3("Velocity",   (float*)&buffer->data.velocity, 0.1f);
    ImGui::DragFloat("Distance",    (float*)&buffer->data.distance);
    ImGui::DragFloat("Turbulence",  (float*)&buffer->data.turbulence, 0.1f, 0.0f, 10.0f);
    ImGui::DragFloat3("Origin",     (float*)&buffer->data.origin, 0.1f);
    ImGui::DragFloat3("Size",       (float*)&buffer->data.size, 0.1f);
    ImGui::ColorEdit3("Color",      (float*)&buffer->data.color);
}

void Snow::SetVertex()
{
    drawCount = MAX_COUNT;

    vertices.resize(drawCount);

    for (UINT i = 0; i < drawCount; i++)
    {
        float size = Random(0.5f, 2.0f);

        vertices[i].size = { size, size };

        vertices[i].pos.x = Random(-buffer->data.size.x, +buffer->data.size.x);
        vertices[i].pos.y = Random(-buffer->data.size.y, +buffer->data.size.y);
        vertices[i].pos.z = Random(-buffer->data.size.z, +buffer->data.size.z);

        vertices[i].random.x = Random(0.0f, 1.0f);
        vertices[i].random.y = Random(0.0f, 1.0f);
    }

    vertexBuffer = new VertexBuffer(vertices);
}