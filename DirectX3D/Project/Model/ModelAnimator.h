#pragma once

/* Model에 animation이 있는 버전 */
// 가만히 있는 animation이 없는 애들은 Model
// ModelAnimator는 animation이 있는 물체들 (ex - character)
class ModelAnimator : public Model
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

	class FrameBuffer : public ConstBuffer<Motion>
	{
	public:
		FrameBuffer() : ConstBuffer(data) {}

		Motion data{};
	};

public:
	ModelAnimator(string name);
	~ModelAnimator();

	void Update();
	void Render();

	void Debug();

	void ReadClip(string file, UINT clipIndex = 0);

	void PlayClip(int clipIndex, float speed = 1.f, float takeTime = 0.2f);

	Matrix GetTransformByBone(string boneName);

	ModelClip* GetClip(UINT index) const { return clips[index]; }

private:

	void CreateClipTransform(UINT clipIndex);
	void UpdateFrame();

protected:
	void CreateTexture();

protected:
	vector<ModelClip*> clips{};

	ClipTransform* clipTransforms{};
	ClipTransform* nodeTransforms{};

	// 실질적으로 png 이미지로 뽑은 texture가 아님 / 내부적으로 만들고 사용하는 texture
	ID3D11Texture2D*			texture{};
	ID3D11ShaderResourceView*	srv{};

	bool isPlay = true;

private:
	FrameBuffer* buffer{};
};
