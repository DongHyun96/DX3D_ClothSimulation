#include "Framework.h"
#include "TutorialScene.h"


TutorialScene::TutorialScene()
{
    cube = new Cube();
    //cube->GetMaterial()->SetShader(L"16_Light");
    cube2 = new Cube(Vector4(0, 1, 0, 1));

    //cube2->translation.x = 0.01f;

    //cube2->SetParent(cube);
    //cube->SetPivot(Vector3(0.5f, 0.5f, 0.5f));

    cube->translation = { 2, 0, 0 };
}

TutorialScene::~TutorialScene()
{
    delete cube;
    delete cube2;

}

void TutorialScene::Update()
{
    cube->Update();
    cube2->Update();

    //cube->rotation.y += 0.0001f;


}

void TutorialScene::Render()
{
    cube->Render();
    //cube2->Render();

}

void TutorialScene::PreRender()
{
}

void TutorialScene::PostRender()
{
    //ImGui::Text("HelloWorld!");
    cube->Debug();
}
