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

	void Init();

private:

	Cloth* cloth{};

	//vector<ColliderSphere*> obstacles{};

	map<ColliderSphere*, Sphere*> obstacles{}; // <Collider, Rendering Sphere>

private:

	// Differential equation 풀이용 timeStep -> 한 Update tick 안에서 dt를 작게 쪼개는 용도
	// Euler method으로 입자의 현 velocity와 위치를 계산할 때, raw한 delta time 사용 시 오차가 상당히 커서 시뮬레이션이 터져버림
	const UINT PHYSICS_TIMESTEP = 1;

};
