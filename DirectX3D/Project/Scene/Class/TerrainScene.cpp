#include "Framework.h"
#include "TerrainScene.h"


TerrainScene::TerrainScene()
{
	terrain = new Terrain(L"HeightMap/HeightMap.png", L"HeightMap/A.png");

	terrain->SetSecondDiffuseMap(L"Solid/Black.png");

	terrain->GetMaterial()->Load(L"_TextData/FieldStone.mat");
	terrain->GetMaterial()->SetShader(L"08_Terrain");

	//terrain->GetMaterial()->SetDiffuseMap(L"LandScape/Fieldstone_DM.tga");
	//terrain->GetMaterial()->SetSpecularMap(L"LandScape/fieldstone_SM.tga");
	//terrain->GetMaterial()->SetNormalMap(L"LandScape/fieldstone_NM.tga");
	cube = new Cube;
}

TerrainScene::~TerrainScene()
{
	delete terrain;

	delete cube;
}

void TerrainScene::Update()
{
	terrain->Update();

	cube->Update();

	if (KEY_PRESS('W'))
	{
		cube->translation += cube->GetForwardVector() * 10.f * DELTA_TIME;
	}
	if (KEY_PRESS('S'))
	{
		cube->translation -= cube->GetForwardVector() * 10.f * DELTA_TIME;
	}
	if (KEY_PRESS('A'))
	{
		cube->rotation.y -= DELTA_TIME;
	}
	if (KEY_PRESS('D'))
	{
		cube->rotation.y += DELTA_TIME;
	}

	cube->translation.y = LERP(cube->translation.y, terrain->GetHeight(cube->translation) + 0.5f, 20.f * DELTA_TIME);
}

void TerrainScene::Render()
{
	terrain->Render();

	cube->Render();
}

void TerrainScene::PreRender()
{
}

void TerrainScene::PostRender()
{
	terrain->GetMaterial()->Debug();
}

