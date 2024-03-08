#pragma once
class Spark : public Particle
{
private:
	struct VertexParticle
	{
		Vector3 position{};
		Vector2 size{};
		Vector3 velocity{};
	};

public:
	Spark(wstring file, bool isAdditive = false);
	~Spark();

	void Update() override;
	void Render() override;
	void Debug()  override;

	void Play(Vector3 position) override;

private:

	void SetVertex() override;

	void UpdateParticle();

private:

	vector<VertexParticle> vertices{};

	FloatValueBuffer* valueBuffer{};		// 실질적으로 값 두개를 사용할 것임(time, duration)
	FloatValueBuffer* startColorBuffer{};
	FloatValueBuffer* endColorBuffer{};

	float minRadius = 5.f;
	float maxRadius = 10.f;

	float minSize = 0.1f;
	float maxSize = 1.f;

};
