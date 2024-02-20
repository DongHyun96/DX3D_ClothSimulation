#include "Framework.h"
#include "IndexBuffer.h"


IndexBuffer::IndexBuffer(vector<UINT>& indices)
{
    D3D11_BUFFER_DESC desc = {};

    desc.ByteWidth  = sizeof(UINT) * indices.size();
    desc.Usage      = D3D11_USAGE_DEFAULT;
    desc.BindFlags  = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = indices.data();

    DEVICE->CreateBuffer(&desc, &initData, &indexBuffer);
}

IndexBuffer::~IndexBuffer()
{
    indexBuffer->Release();
}

void IndexBuffer::IASetBuffer()
{
    DC->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void IndexBuffer::UpdateIndex(void* data, UINT dataCount)
{
    DC->UpdateSubresource(indexBuffer, 0, nullptr, data, sizeof(UINT), dataCount);
}
