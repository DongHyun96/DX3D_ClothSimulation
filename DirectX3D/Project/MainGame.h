#pragma once
class MainGame
{
public:

	MainGame();
	~MainGame();

	void Update(); // 정보를 갱신하는 함수
	void Render(); // 화면을 갱신하는 함수

	void PreRender();
	void PostRender();

private:

	void Initialize();
	void Release();

private:

	//Scene* scene{};

};
