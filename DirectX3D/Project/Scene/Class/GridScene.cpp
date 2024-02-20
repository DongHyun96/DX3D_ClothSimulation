#include "Framework.h"
#include "GridScene.h"


GridScene::GridScene()
{
	grid = new Grid;
}

GridScene::~GridScene()
{
	delete grid;
}

void GridScene::Update()
{
	grid->Update();
}

void GridScene::Render()
{
	grid->Render(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void GridScene::PreRender()
{
}

void GridScene::PostRender()
{
}
