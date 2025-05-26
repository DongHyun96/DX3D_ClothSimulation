#pragma once
class Shader
{
protected:
	virtual ~Shader();

public:
	virtual void SetShader() = 0;

	static void Delete();

	static class VertexShader*		AddVS(wstring file);
	static class PixelShader*		AddPS(wstring file);
	static class ComputeShader*		AddCS(wstring file);
	static class GeometryShader*	AddGS(wstring file);
	static class HullShader*		AddHS(wstring file);
	static class DomainShader*		AddDS(wstring file);

	wstring GetPath() const { return path; }

protected:

	ID3DBlob* blob;

	static map<wstring, Shader*> shaders;

	wstring path{};

};
