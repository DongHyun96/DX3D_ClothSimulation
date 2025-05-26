#pragma once
class Particle
{
public:
	Particle();
	virtual ~Particle();

	virtual void Update() {}
	virtual void Render();
	virtual void Debug() {}

	virtual void Play(Vector3 position);

	void Stop();

private:

	virtual void SetVertex() = 0;

protected:

	Material*		material{};
	VertexBuffer*	vertexBuffer{};
	GeometryShader* geometryShader{};

	const UINT MAX_COUNT = 1000;

	UINT drawCount = MAX_COUNT;

	bool isAdditive = false;
	bool isActive	= false;

	Vector3 position{};

};