#pragma once

class Rain : public Particle
{
public:
	Rain(wstring file);
	~Rain();

	virtual void Update() override;
	virtual void Render() override;
	virtual void Debug()  override;

private:
	virtual void SetVertex() override;

private:
	vector<VertexTexture> vertices;

	WeatherBuffer* buffer;
};