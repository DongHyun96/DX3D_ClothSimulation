#pragma once
class StructuredBuffer
{
public:
	StructuredBuffer
	(
		void*	inputData,
		UINT	inputStride,
		UINT	inputCount,
		UINT	outputStride,
		UINT	outputCount
	);

	~StructuredBuffer();

	void Copy(void* data, UINT size);

	void UpdateInput(void* data);

	ID3D11ShaderResourceView*&	GetSRV() { return srv; } // Pointer의 Reference
	ID3D11UnorderedAccessView*&	GetUAV() { return uav; }

private:

	void CreateInput();
	void CreateSRV();
	void CreateOutput();
	void CreateUAV(); // Unordered Access View ( GPU가 CPU에 데이터를 넘길 때의 view)
	void CreateResult();

private:

	ID3D11Buffer* input{};
	ID3D11ShaderResourceView* srv{};

	ID3D11Buffer* output{};
	ID3D11UnorderedAccessView* uav{};

	ID3D11Buffer* result{};

	void* inputData{};

	UINT inputStride{}, inputCount{};
	UINT outputStride{}, outputCount{};

};
