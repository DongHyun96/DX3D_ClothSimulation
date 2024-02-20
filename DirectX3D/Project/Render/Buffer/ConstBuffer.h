#pragma once


template<typename T>
class ConstBuffer
{
public:
	ConstBuffer(T& data);

	virtual ~ConstBuffer();

	void SetVSBuffer(UINT slot);
	void SetPSBuffer(UINT slot);
	void SetCSBuffer(UINT slot);
	void SetDSBuffer(UINT slot);
	void SetGSBuffer(UINT slot);

	void UpdateSubresource();

private:
	ID3D11Buffer* constantBuffer;

	T& data;

	D3D11_MAPPED_SUBRESOURCE subResource;
};

template<typename T>
inline ConstBuffer<T>::ConstBuffer(T& data)
	:data(data)
{
	D3D11_BUFFER_DESC desc = {};
	
	desc.ByteWidth		= sizeof(T);
	desc.Usage			= D3D11_USAGE_DYNAMIC;
	desc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	DEVICE->CreateBuffer(&desc, nullptr, &constantBuffer);
}

template<typename T>
inline ConstBuffer<T>::~ConstBuffer()
{
	constantBuffer->Release();
}

template<typename T>
inline void ConstBuffer<T>::SetVSBuffer(UINT slot)
{
	UpdateSubresource();

	DC->VSSetConstantBuffers(slot, 1, &constantBuffer);
}

template<typename T>
inline void ConstBuffer<T>::SetPSBuffer(UINT slot)
{
	UpdateSubresource();

	DC->PSSetConstantBuffers(slot, 1, &constantBuffer);

}

template<typename T>
inline void ConstBuffer<T>::SetCSBuffer(UINT slot)
{
	UpdateSubresource();

	DC->CSSetConstantBuffers(slot, 1, &constantBuffer);
}

template<typename T>
inline void ConstBuffer<T>::SetDSBuffer(UINT slot)
{
	UpdateSubresource();

	DC->DSSetConstantBuffers(slot, 1, &constantBuffer);
}

template<typename T>
inline void ConstBuffer<T>::SetGSBuffer(UINT slot)
{
	UpdateSubresource();

	DC->GSSetConstantBuffers(slot, 1, &constantBuffer);
}

template<typename T>
inline void ConstBuffer<T>::UpdateSubresource()
{
	//DC->UpdateSubresource(constantBuffer, 0, nullptr, &data, 0, 0);

	DC->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

	memcpy(subResource.pData, &data, sizeof(T));

	DC->Unmap(constantBuffer, 0);
}

