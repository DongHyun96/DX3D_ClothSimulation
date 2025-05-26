#include "Framework.h"
#include "ProgressBar.h"


ProgressBar::ProgressBar(wstring front, wstring back)
{
	material->SetDiffuseMap(front);
	material->SetShader(L"11_ProgressBar");

	backImage = Texture::Add(back);

	buffer = new FloatValueBuffer;
	buffer->data.values[0] = 1.f;
}

ProgressBar::~ProgressBar()
{
	delete buffer;
}

void ProgressBar::Render()
{
	buffer->SetPSBuffer(10);
	backImage->PSSetShaderResources(10);

	Quad::Render();
}

void ProgressBar::SetValue(float value)
{
	buffer->data.values[0] = value;
}
