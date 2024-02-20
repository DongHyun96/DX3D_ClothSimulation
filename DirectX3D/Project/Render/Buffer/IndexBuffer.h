#pragma once
class IndexBuffer
{
public:
	IndexBuffer(vector<UINT>& indices);
	~IndexBuffer();

	void IASetBuffer();

	void UpdateIndex(void* data, UINT dataCount);

private:

	ID3D11Buffer* indexBuffer{};

};
