#pragma once
class MainGame
{
public:

	MainGame();
	~MainGame();

	void Update(); // ������ �����ϴ� �Լ�
	void Render(); // ȭ���� �����ϴ� �Լ�

	void PreRender();
	void PostRender();

private:

	void Initialize();
	void Release();

private:

	//Scene* scene{};

};
