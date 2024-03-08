#pragma once
class MapTestScene : public Scene
{
public:
	MapTestScene();
	~MapTestScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;

private:

	Model* map{};

};
