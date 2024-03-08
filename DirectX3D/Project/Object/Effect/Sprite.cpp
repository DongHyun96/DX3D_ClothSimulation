#include "Framework.h"
#include "Sprite.h"


Sprite::Sprite(wstring file, Vector2 size, float frameX, float frameY, bool isAdditive, float maxFrameCount)
    :size(size)
{
    this->isAdditive = isAdditive;

    if (maxFrameCount == 0)
        this->maxFrameCount = frameX * frameY;
    else
        this->maxFrameCount = maxFrameCount;

    buffer = new FloatValueBuffer();
    buffer->data.values[0] = frameX;
    buffer->data.values[1] = frameY;

    material->SetShader(L"22_Sprite");
    material->SetDiffuseMap(file);

    geometryShader = Shader::AddGS(L"22_Sprite");

    SetVertex();
}

Sprite::~Sprite()
{
    delete buffer;
}

void Sprite::SetVertex()
{
    drawCount = 1;

    vertex.resize(1);

    vertexBuffer = new VertexBuffer(vertex);
}

void Sprite::Update()
{
    if (!isActive)
        return;

    time += speed * Time::Delta();

    if (time > 0.1f)
    {
        curFrameCount++;

        float curFrameX = curFrameCount % (UINT)buffer->data.values[0];
        float curFrameY = curFrameCount / (UINT)buffer->data.values[0];

        buffer->data.values[2] = curFrameX;
        buffer->data.values[3] = curFrameY;

        time = 0.0f;
    }

    if (curFrameCount >= maxFrameCount)
        Stop();
}

void Sprite::Render()
{
    buffer->SetPSBuffer(10);

    Particle::Render();
}

void Sprite::Debug()
{
}

void Sprite::Play(Vector3 position)
{
    isActive = true;

    time = 0.0f;
    curFrameCount = 0;

    vertex[0].pos = position;
    vertex[0].uv = size;

    vertexBuffer->UpdateVertex(vertex.data(), drawCount);
}