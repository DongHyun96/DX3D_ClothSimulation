#pragma once

class DomainShader : public Shader
{
	friend class Shader;
private:
	DomainShader(wstring file);
	~DomainShader();

public:
	virtual void SetShader() override;

private:
	ID3D11DomainShader* shader;
};