#pragma once
class Spring
{
public:
	Spring(); // Instancing용 기본 생성자
	Spring(RigidSphere* p1, RigidSphere* p2, float k = DEFAULT_K_SPRING, Vector4 color = {1, 0, 0, 1});
	~Spring();

	void Update();
	void Render();
	void RenderInstanced(const UINT& count);

	void AddForceToParticles();

	void Debug();

	void SetShader(wstring file) { body->GetMaterial()->SetShader(file); }

	Matrix& GetWorld() const { return body->GetWorld(); }

private:

	void UpdateBody();

private:

	Cube* body{};

	Vector4 color{};

	RigidSphere* particle1{};
	RigidSphere* particle2{};

	float r{}; 
	float k{}; // 스프링 상수

};
