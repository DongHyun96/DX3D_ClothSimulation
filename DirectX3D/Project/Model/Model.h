#pragma once

class Model : public Transform
{
protected:
	struct InstanceData
	{
		Matrix world = XMMatrixIdentity();
		UINT   index = 0;
	};

public:
	Model(string name);
	virtual ~Model();

	void Update() override;

	void Render();
	void RenderInstanced(UINT instanceCount);

	void Debug();

	void SetShader(wstring file);

	void AttachToBone(class ModelAnimator* model, string boneName);



private:

	void ReadMaterial();
	void ReadMesh();

	void UpdateSockets();
	
protected:

	vector<Material*>		materials{};
	vector<ModelMesh*>		meshes{};

	vector<NodeData>		nodes{};
	vector<BoneData>		bones{};

	map<string, UINT>		boneMap{};

	map<string, Transform*> sockets{};
};
