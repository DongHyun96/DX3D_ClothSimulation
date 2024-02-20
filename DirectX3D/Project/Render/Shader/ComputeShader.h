#pragma once
class ComputeShader : public Shader
{
	friend class Shader;

private:
	ComputeShader(wstring file);
	~ComputeShader();

public:
	void SetShader() override;

private:
	ID3D11ComputeShader* computeShader;
};
