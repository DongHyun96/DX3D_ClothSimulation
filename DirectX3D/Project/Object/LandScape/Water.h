#pragma once
class Water : public GameObject<VertexTexture>
{
public:
	Water(wstring normalFile, Vector2 size = Vector2(100, 100));
	~Water();

	void Update();
	void Render();
	void Debug();

	void SetReflection();
	void SetRefraction();


private:

	void CreateMesh() override;

private:

	Vector2 size{};

	Reflection* reflection{};
	Refraction* refraction{};

};
