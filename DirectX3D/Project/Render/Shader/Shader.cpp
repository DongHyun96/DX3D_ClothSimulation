#include "Framework.h"
#include "Shader.h"

map<wstring, Shader*> Shader::shaders{};

Shader::~Shader()
{
	blob->Release();
}

void Shader::Delete()
{
	for (pair<wstring, Shader*> shader : shaders)
		delete shader.second;

	shaders.clear();
}

VertexShader* Shader::AddVS(wstring file)
{
	wstring path = file;

	wstring key = L"VS" + file;

	if (!StartsWith(file, L"_Shader/"))
		file = L"_Shader/" + file + L".hlsl";

	assert(PathFileExists(file.c_str()));

	if (shaders.count(key) > 0)
		return (VertexShader*)shaders[key];

	shaders.emplace(key, new VertexShader(file));

	shaders[key]->path = path;

	return (VertexShader*)shaders[key];
}

PixelShader* Shader::AddPS(wstring file)
{
	wstring path = file;

	wstring key = L"PS" + file;

	if (!StartsWith(file, L"_Shader/"))
		file = L"_Shader/" + file + L".hlsl";

	assert(PathFileExists(file.c_str()));

	if (shaders.count(key) > 0)
		return (PixelShader*)shaders[key];

	shaders.emplace(key, new PixelShader(file));

	shaders[key]->path = path;

	return (PixelShader*)shaders[key];
}

ComputeShader* Shader::AddCS(wstring file)
{
	wstring path = file;

	wstring key = L"CS" + file;

	if (!StartsWith(file, L"_Shader/"))
		file = L"_Shader/" + file + L".hlsl";

	assert(PathFileExists(file.c_str()));

	if (shaders.count(key) > 0)
		return (ComputeShader*)shaders[key];

	shaders.emplace(key, new ComputeShader(file));

	shaders[key]->path = path;

	return (ComputeShader*)shaders[key];
}

GeometryShader* Shader::AddGS(wstring file)
{
	wstring path = file;

	wstring key = L"GS" + file;

	if (!StartsWith(file, L"_Shader/"))
		file = L"_Shader/" + file + L".hlsl";

	assert(PathFileExists(file.c_str()));

	if (shaders.count(key) > 0)
		return (GeometryShader*)shaders[key];

	shaders.emplace(key, new GeometryShader(file));

	shaders[key]->path = path;

	return (GeometryShader*)shaders[key];
}
