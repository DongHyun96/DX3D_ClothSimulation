#pragma once
class Grid : public GameObject<VertexColor>
{
public:
	Grid();
	~Grid();

private:
	void CreateMesh() override;

private:
	Vector2 size = { 1000, 1000 };

};
