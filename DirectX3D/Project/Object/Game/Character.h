#pragma once
class Character : public ColliderCapsule
{
public:
	Character(string name, float radius = 1.f, float height = 2.f);
	~Character();

	void Update();
	void Render();
	void Debug();

	ModelAnimator* GetModel() const { return model; }

private:
	ModelAnimator* model{};
};
