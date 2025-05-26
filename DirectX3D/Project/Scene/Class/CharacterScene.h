#pragma once
class CharacterScene : public Scene
{
public:
	CharacterScene();
	~CharacterScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

	Knight* player{};

};
