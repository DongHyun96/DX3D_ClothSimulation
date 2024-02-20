#pragma once
class ModelAnimatorInstancing : public ModelAnimator
{

protected:

	struct Frame
	{
		int		clipIndex{};
		UINT	frameIndex{};
		float	time{};
		float	speed = 1.f;
	};

	struct Motion
	{
		Motion()
		{
			nextFrame.clipIndex = -1;
		}

		float takeTime = 0.2f;
		float tweenTime{}; // clip과 clip 사이의 tween time
		float runningTime{};
		float padding{};

		Frame curFrame{};
		Frame nextFrame{}; // 다음 모션의 Frame
	};

	struct Data
	{
		Motion motion[MAX_INSTANCE];
	};

	class FrameInstancingBuffer : public ConstBuffer<Data>
	{
	public:
		FrameInstancingBuffer() : ConstBuffer(data) {}

		Data data{};
	};

public:
	ModelAnimatorInstancing(string name);
	~ModelAnimatorInstancing();

	void Update();
	void Render();

	void Debug();

	Transform* AddTransform();

	void PlayClip(UINT instanceIndex, int clipIndex, float speed = 1.f, float takeTime = 0.1f);

private:

	void UpdateFrame(UINT instanceIndex);

private:

	vector<Transform*> transforms{};

	vector<InstanceData> instanceDatas{};

	VertexBuffer* instanceBuffer{};

	UINT drawCount{};

	FrameInstancingBuffer* buffer{};

	int instanceIndex{};
	int clipIndex{};
};
