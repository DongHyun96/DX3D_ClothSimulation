#pragma once
class TextureCube : public Transform
{
	enum Side
	{
		FRONT,
		BACK,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM
	};

public:
	TextureCube();
	~TextureCube();

	void Update();
	void Render();

	void SetDiffuseMap(wstring file);
	void SetSpecularMap(wstring file);
	void SetNormalMap(wstring file);

	void SetShader(wstring file);

	Quad* GetFrontQuad() const { return quads[FRONT]; }

private:
	void CreateQuads();

private:

	vector<Quad*> quads{};

};
