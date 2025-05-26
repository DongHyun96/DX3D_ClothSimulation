#pragma once
class Sprite : public Particle
{
public:
	Sprite(wstring file, Vector2 size, float frameX, float frameY, bool isAdditive = false, float maxFrameCount = 0);
	~Sprite();


	void Update() override;
	void Render() override;
	void Debug()  override;

	void Play(Vector3 position) override;

private:
	virtual void SetVertex() override;

private:
	FloatValueBuffer* buffer;

	vector<VertexTexture> vertex;

	float time = 0.0f;
	float speed = 1.0f;

	UINT curFrameCount = 0;
	UINT maxFrameCount = 0;

	Vector2 size;
};