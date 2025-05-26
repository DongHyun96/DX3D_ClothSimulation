#pragma once
class GridScene : public Scene
{
public:
	GridScene();
	~GridScene();

	void Update()		override;

	void Render()		override;
	void PreRender()	override;
	void PostRender()	override;



private:

	Grid* grid{};
};
