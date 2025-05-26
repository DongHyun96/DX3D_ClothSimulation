#include "Framework.h"
#include "VertexBuffer.h"


VertexBuffer::~VertexBuffer()
{
    vertexBuffer->Release();
}

void VertexBuffer::IASetBuffer(D3D11_PRIMITIVE_TOPOLOGY topology)
{
    DC->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    DC->IASetPrimitiveTopology(topology);
}

void VertexBuffer::IASetBuffer(UINT slot, D3D11_PRIMITIVE_TOPOLOGY topology)
{
    DC->IASetVertexBuffers(slot, 1, &vertexBuffer, &stride, &offset);
    DC->IASetPrimitiveTopology(topology);
}

void VertexBuffer::UpdateVertex(void* vertices, UINT count)
{
    DC->UpdateSubresource(vertexBuffer, 0, nullptr, vertices, stride, count);
}
