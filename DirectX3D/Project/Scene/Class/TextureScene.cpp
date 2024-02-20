#include "Framework.h"
#include "TextureScene.h"

TextureScene::TextureScene()
{
	quad = new Quad;
	quad->GetMaterial()->SetDiffuseMap(L"Landscape/Box.png");

	cube = new TextureCube;
	cube->SetDiffuseMap(L"Landscape/Box.png");
	cube->SetName("TextureCubeExample");
	cube->translation.x = -2;

	sphere = new Sphere;
	sphere->GetMaterial()->SetDiffuseMap(L"LandScape/FieldStone_DM.tga");
	sphere->GetMaterial()->SetSpecularMap(L"LandScape/FieldStone_SM.tga");
	sphere->GetMaterial()->SetNormalMap(L"LandScape/FieldStone_NM.tga");

}

TextureScene::~TextureScene()
{
	delete quad;
	delete cube;
	delete sphere;
}

void TextureScene::Update()
{
	quad->Update();
	cube->Update();
	sphere->Update();
}

void TextureScene::Render()
{
	//quad->Render();
	cube->Render();
	sphere->Render();
}

void TextureScene::PreRender()
{
}

void TextureScene::PostRender()
{
	cube->Debug();
	sphere->Debug();
}
