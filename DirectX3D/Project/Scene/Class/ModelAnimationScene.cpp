#include "Framework.h"
#include "ModelAnimationScene.h"


ModelAnimationScene::ModelAnimationScene()
{
	model = new ModelAnimator("Knight");

	model->ReadClip("Dancing Twerk");
	model->ReadClip("Goofy Running");
	model->ReadClip("Sad Idle");

	model->GetClip(0)->AddEvent(0.5f, bind(&ModelAnimationScene::PlayClip, this, 1));
	model->GetClip(1)->AddEvent(0.5f, bind(&ModelAnimationScene::PlayClip, this, 2));
	model->GetClip(2)->AddEvent(0.5f, bind(&ModelAnimationScene::PlayClip, this, 0));

	sword = new Model("Sword");
	sword->AttachToBone(model, "mixamorig:RightHand");
}


ModelAnimationScene::~ModelAnimationScene()
{
	delete model;
	delete sword;
}

void ModelAnimationScene::Update()
{
	model->Update();
	sword->Update();

}

void ModelAnimationScene::Render()
{
	//sword->GetWorld() = model->GetTransformByBone("mixamorig:RightHand");

	model->Render();
	sword->Render();
}

void ModelAnimationScene::PreRender()
{
}

void ModelAnimationScene::PostRender()
{
	model->Debug();

	sword->Debug();
}

void ModelAnimationScene::PlayNextClip()
{
	static int index = 1;

	model->PlayClip(index);

	++index %= 3;
}

void ModelAnimationScene::PlayClip(int index)
{
	model->PlayClip(index, 1.f, 5.f);
}
