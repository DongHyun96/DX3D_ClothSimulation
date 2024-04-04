#include "Framework.h"
#include "TextureScene.h"

TextureScene::TextureScene()
{
	quad = new Quad;
	quad->GetMaterial()->SetDiffuseMap(L"Landscape/Box.png");

	cube = new Cube;
	cube->translation.x = -4;

	textureCube = new TextureCube;
	textureCube->SetDiffuseMap(L"Default/TemplateGrid_albedo.png");
	textureCube->SetNormalMap(L"Default/TemplateGrid_normal.png");

	//textureCube->SetDiffuseMap(L"LandScape/FieldStone_DM.tga");
	//textureCube->SetSpecularMap(L"LandScape/FieldStone_SM.tga");
	//textureCube->SetNormalMap(L"LandScape/FieldStone_NM.tga");


	textureCube->SetName("TextureCubeExample");
	textureCube->translation.x = -2;
	textureCube->SetShader(L"16_Light");
	//textureCube->SetShader(L"05_NormalMapping");


	sphere = new Sphere;
	//sphere->GetMaterial()->SetDiffuseMap(L"Default/TemplateGrid_albedo.png");
	//sphere->GetMaterial()->SetNormalMap(L"Default/TemplateGrid_orm.png");
	sphere->GetMaterial()->SetShader(L"16_Light");
	//sphere->GetMaterial()->SetDiffuseMap(L"LandScape/FieldStone_DM.tga");
	//sphere->GetMaterial()->SetSpecularMap(L"LandScape/FieldStone_SM.tga");
	//sphere->GetMaterial()->SetNormalMap(L"LandScape/FieldStone_NM.tga");
}

TextureScene::~TextureScene()
{
	delete quad;
	delete textureCube;
	delete sphere;
	delete cube;
}

void TextureScene::Update()
{
	quad->Update();
	textureCube->Update();
	sphere->Update();
	cube->Update();
}

void TextureScene::Render()
{
	//quad->Render();
	textureCube->Render();
	sphere->Render();
	cube->Render();
}

void TextureScene::PreRender()
{
}

void TextureScene::PostRender()
{
	textureCube->Debug();
	
	sphere->Debug();
	sphere->GetMaterial()->Debug();

	cube->Debug();
	cube->GetMaterial()->Debug();
}
