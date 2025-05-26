#pragma once
class ModelAnimationScene : public Scene
{
public:
	ModelAnimationScene();
	~ModelAnimationScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

	void PlayNextClip();

	void PlayClip(int index);

private:
	
	ModelAnimator* model{};

	Model* sword{};
};
