#pragma once

class ModelMesh : public Mesh
{
public:
	ModelMesh(string name, UINT materialIndex, vector<ModelVertex>& vertices, vector<UINT>& indices);
	~ModelMesh();

	UINT GetMaterialIndex() const { return materialIndex; }

	void Render();
	void RenderInstanced(UINT instanceCount);

private:
	
	string name{};

	UINT materialIndex{};

	vector<ModelVertex> vertices{};
	vector<UINT> indices{};
};
