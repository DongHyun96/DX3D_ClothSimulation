#pragma once

/* Animation 1�� (.clip ����) */
class ModelClip
{
	friend class ModelAnimator;
	friend class ModelAnimatorInstancing;

public:
	ModelClip();
	~ModelClip();
	
	KeyFrame* GetKeyFrames(string boneName) { return keyFrames[boneName]; }

	void Init();
	void Execute(float ratio);

	void AddEvent(float ratio, function<void()> Event);

private:
	string name{};

	UINT frameCount{}; // �� frame ����

	float ticksPerSecond{};
	float duration{};

	float playTime{};

	unordered_map<string, KeyFrame*> keyFrames{};

	// Anim Notifier | key - �帥�ð�ratio
	multimap<float, function<void()>> events{};

	// ���� events�� ��ȸ�ϱ� ���� iterator
	multimap<float, function<void()>>::iterator eventsIterator;

	float ratio{};
};
