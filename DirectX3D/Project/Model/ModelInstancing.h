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

private:

	vector<Transform*> transforms{};

	vector<InstanceData> instanceDatas{};

	VertexBuffer* instanceBuffer{};

	UINT drawCount{};
};
