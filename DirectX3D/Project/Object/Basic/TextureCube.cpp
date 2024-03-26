#include "Framework.h"
#include "TextureCube.h"


TextureCube::TextureCube()
{
	CreateQuads();
}

TextureCube::~TextureCube()
{
	for (Quad* quad : quads)
		delete quad;

	quads.clear();
}

void TextureCube::Update()
{
	Transform::Update();

	for (Quad* quad : quads)
		quad->Update();
}

void TextureCube::Render()
{
	for (Quad* quad : quads)
		quad->Render();
}

void TextureCube::SetDiffuseMap(wstring file)
{
	for (Quad* quad : quads)
		quad->GetMaterial()->SetDiffuseMap(file);
}

void TextureCube::SetShader(wstring file)
{
	for (Quad* quad : quads)
		quad->GetMaterial()->SetShader(file);
}

void TextureCube::CreateQuads()
{
	// STL : resize, reserve
	// resize - �޸𸮸� �ش� �����ŭ �Ҵ��ϰ� �� ĭ�� �ʱ�ȭ����
	// reserve - ���� �޸� ������ ������ ���� �ƴ�

	quads.resize(6);

	quads[FRONT] = new Quad;
	quads[FRONT]->SetParent(this);
	quads[FRONT]->translation.z = -0.5f;

	quads[BACK] = new Quad;
	quads[BACK]->SetParent(this);
	quads[BACK]->translation.z = +0.5f;
	quads[BACK]->rotation.x = XM_PI;

	quads[LEFT] = new Quad;
	quads[LEFT]->SetParent(this);
	quads[LEFT]->translation.x = -0.5f;
	quads[LEFT]->rotation.y = +XM_PIDIV2;

	quads[RIGHT] = new Quad;
	quads[RIGHT]->SetParent(this);
	quads[RIGHT]->translation.x = +0.5f;
	quads[RIGHT]->rotation.y = -XM_PIDIV2;

	quads[BOTTOM] = new Quad;
	quads[BOTTOM]->SetParent(this);
	quads[BOTTOM]->translation.y = +0.5f;
	quads[BOTTOM]->rotation.x = +XM_PIDIV2;

	quads[TOP] = new Quad;
	quads[TOP]->SetParent(this);
	quads[TOP]->translation.y = -0.5f;
	quads[TOP]->rotation.x = -XM_PIDIV2;

}
