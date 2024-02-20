#pragma once

/* Model�� animation�� �ִ� ���� */
// ������ �ִ� animation�� ���� �ֵ��� Model
// ModelAnimator�� animation�� �ִ� ��ü�� (ex - character)
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
		float tweenTime{}; // clip�� clip ������ tween time
		float runningTime{};
		float padding{};

		Frame curFrame{};
		Frame nextFrame{}; // ���� ����� Frame
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

	// ���������� png �̹����� ���� texture�� �ƴ� / ���������� ����� ����ϴ� texture
	ID3D11Texture2D*			texture{};
	ID3D11ShaderResourceView*	srv{};

	bool isPlay = true;

private:
	FrameBuffer* buffer{};
};
