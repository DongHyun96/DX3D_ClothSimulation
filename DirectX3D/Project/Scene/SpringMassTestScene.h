#pragma once

/*
	�õ��غ����� ����ȭ

	1. Release

	2. Rigid Sphere & Spring instancing
	-> Update ����ȭ���� �ؾ� �ɵ�?

	������ Ȯ���Ǹ� step�� ���� �־ �Ǵ���

*/


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

	void Init();

private:

	Quad* floor{};

	Cloth* cloth{};

	vector<ColliderSphere*> obstacles{};

};
