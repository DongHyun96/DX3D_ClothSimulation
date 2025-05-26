#pragma once

/* Animation 1개 (.clip 파일) */
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

	UINT frameCount{}; // 총 frame 갯수

	float ticksPerSecond{};
	float duration{};

	float playTime{};

	unordered_map<string, KeyFrame*> keyFrames{};

	// Anim Notifier | key - 흐른시간ratio
	multimap<float, function<void()>> events{};

	// 위의 events를 순회하기 위한 iterator
	multimap<float, function<void()>>::iterator eventsIterator;

	float ratio{};
};
