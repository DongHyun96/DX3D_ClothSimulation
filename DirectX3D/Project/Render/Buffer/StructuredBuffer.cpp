#include "Framework.h"
#include "StructuredBuffer.h"


StructuredBuffer::StructuredBuffer(void* inputData, UINT inputStride, UINT inputCount, UINT outputStride, UINT outputCount)
	:inputData(inputData), inputStride(inputStride), inputCount(inputCount),
	outputStride(outputStride), outputCount(outputCount)
{
	CreateInput();
	CreateSRV();
	CreateOutput();
	CreateUAV();
	CreateResult();
}

StructuredBuffer::~StructuredBuffer()
{
	result->Release();
	uav->Release();
	output->Release();
	srv->Release();
	input->Release();
}

void StructuredBuffer::Copy(void* data, UINT size)
{
	DC->CopyResource(result, output);

	D3D11_MAPPED_SUBRESOURCE subResource{};

	DC->Map(result, 0, D3D11_MAP_READ, 0, &subResource);

	memcpy(data, subResource.pData, size);

	DC->Unmap(result, 0);
}

void StructuredBuffer::UpdateInput(void* data)
{
	DC->UpdateSubresource(input, 0, nullptr, data, inputStride, inputCount);

	srv->Release();

	CreateSRV();
}

void StructuredBuffer::CreateInput()
{
	D3D11_BUFFER_DESC desc{};

	desc.Usage					= D3D11_USAGE_DEFAULT;
	desc.ByteWidth				= inputStride * inputCount;
	desc.BindFlags				= D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags				= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride	= inputStride;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = inputData;

	DEVICE->CreateBuffer(&desc, &data, &input);
}

void StructuredBuffer::CreateSRV()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc{};

	desc.Format				= DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension		= D3D11_SRV_DIMENSION_BUFFEREX;
	desc.Buffer.NumElements = inputCount;
	
	DEVICE->CreateShaderResourceView(input, &desc, &srv);
}

void StructuredBuffer::CreateOutput()
{
	D3D11_BUFFER_DESC desc{};

	desc.Usage		= D3D11_USAGE_DEFAULT;
	desc.ByteWidth	= outputStride * outputCount;
	desc.BindFlags	= D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags	= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	desc.StructureByteStride = outputStride;

	DEVICE->CreateBuffer(&desc, nullptr, &output);
}

void StructuredBuffer::CreateUAV()
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC desc{};

	desc.Format					= DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension			= D3D11_UAV_DIMENSION_BUFFER;
	desc.Buffer.NumElements		= outputCount;

	DEVICE->CreateUnorderedAccessView(output, &desc, &uav);
}

void StructuredBuffer::CreateResult()
{
	D3D11_BUFFER_DESC desc{};

	output->GetDesc(&desc);
	desc.Usage				= D3D11_USAGE_STAGING;
	desc.BindFlags			= 0;
	desc.MiscFlags			= 0;
	desc.CPUAccessFlags		= D3D11_CPU_ACCESS_READ;

	DEVICE->CreateBuffer(&desc, nullptr, &result);
}
