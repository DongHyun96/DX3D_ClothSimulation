#pragma once
class Texture
{
public:
	Texture(ID3D11ShaderResourceView* srv, ScratchImage& image);
	~Texture();

	// ���� Add�� file�� �޾ƿͼ� srv�� ����� ��
	static Texture* Add(wstring file);
	static Texture* Add(wstring file, wstring key);

	static Texture* Add(wstring key, ID3D11ShaderResourceView* srv);

	static void Delete();

	void PSSetShaderResources(UINT slot);
	void DSSetShaderResources(UINT slot);

	// Height map�� ���� �� ���
	vector<Vector4> ReadPixels();

	Vector2 GetSize() const { return Vector2(image.GetMetadata().width, image.GetMetadata().height); }

	ID3D11ShaderResourceView* GetSRV() const { return srv; }

	wstring GetPath() const { return path; }

private:
	
	static map<wstring, Texture*> textures;

	ScratchImage image{};

	ID3D11ShaderResourceView* srv{};

	wstring path{};

};
