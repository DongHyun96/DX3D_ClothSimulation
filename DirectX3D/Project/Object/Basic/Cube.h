#pragma once


class Cube : public GameObject<VertexColorNormal>
{
	typedef VertexColorNormal VertexType;

public:
	Cube(Vector4 color = Vector4(1, 0, 0, 1));
	~Cube();

	void Render();

	void CreateMesh() override;

private:

	Vector4 color{};

};
