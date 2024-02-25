#pragma once
class ModelInstancing : public Model
{
public:
	ModelInstancing(string name);
	~ModelInstancing();

	void Update();
	void Render();

	void Debug();

	Transform* AddTransform();

	void SaveTransforms();
	void LoadTransforms();

	void SetParent(Transform* parent) override;

private:

	vector<Transform*> transforms{};

	vector<InstanceData> instanceDatas{};

	VertexBuffer* instanceBuffer{};

	UINT drawCount{};
};
