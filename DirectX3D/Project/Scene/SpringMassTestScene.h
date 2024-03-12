#pragma once
class SpringMassTestScene : public Scene
{
public:
	SpringMassTestScene();
	~SpringMassTestScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

};
