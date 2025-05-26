#pragma once
class SkyBox
{
public:
	SkyBox(wstring file);
	~SkyBox();

	void Update();
	void Render();

private:
	
	Texture* cubeMap{};

	Sphere* sphere{};

};
