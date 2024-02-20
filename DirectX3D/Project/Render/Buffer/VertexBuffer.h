#pragma once
class VertexBuffer
{
public:
	template<typename T>
	VertexBuffer(vector<T>& vertices);
	~VertexBuffer();

	void IASetBuffer(D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void IASetBuffer(UINT slot, D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void UpdateVertex(void* vertices, UINT count);

private:

	ID3D11Buffer* vertexBuffer{};

	UINT stride{};
	UINT offset{};
};

template<typename T>
inline VertexBuffer::VertexBuffer(vector<T>& vertices)
{
    D3D11_BUFFER_DESC desc = {};

	stride = sizeof(T);
	offset = 0;

    desc.ByteWidth	= stride * vertices.size();
    desc.Usage		= D3D11_USAGE_DEFAULT;
    desc.BindFlags	= D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices.data();

    DEVICE->CreateBuffer(&desc, &initData, &vertexBuffer);
}
