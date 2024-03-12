#pragma once

class HullShader : public Shader
{
	friend class Shader;
private:
	HullShader(wstring file);
	~HullShader();

public:
	virtual void SetShader() override;

private:
	ID3D11HullShader* shader;
};