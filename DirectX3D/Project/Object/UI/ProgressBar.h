#pragma once

class ProgressBar : public Quad
{
public:
	ProgressBar(wstring front, wstring back);
	~ProgressBar();

	void Render();

	void SetValue(float value);

private:
	Texture* backImage{};

	FloatValueBuffer* buffer{};

};
