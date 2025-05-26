#pragma once
class Material
{
public:
	Material();
	Material(wstring file);
	~Material();

	void Set();

	void SetShader(wstring file);

	void SetDiffuseMap(wstring file);
	void SetDiffuseMap(Texture* texture);

	void SetSpecularMap(wstring file);
	void SetNormalMap(wstring file);

	void Debug(string baseDir = "_TextData/");

	void SelectMap(Texture** texture, string mapType, wstring clearFile);

	void SaveMaterial(wstring file);
	void LoadMaterial(wstring file);

	void SaveDialog(string baseDir);
	void LoadDialog(string baseDir);

	void SetName(string name)	{ this->name = name; }
	string GetName() const		{ return name; }

	MaterialBuffer* GetBuffer() const { return buffer; }

private:
	VertexShader* vertexShader{};
	PixelShader* pixelShader{};

	Texture* diffuseMap{};
	Texture* specularMap{};
	Texture* normalMap{};

	string name{};
	static UINT index;

	MaterialBuffer* buffer{};
};
