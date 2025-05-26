#include "Framework.h"
#include "ParticleScene.h"


ParticleScene::ParticleScene()
{
	particle = new Sprite(L"Effect/greenCore_4x4.png", Vector2(30, 30), 4, 4, true);
	//sprite->Play(Vector3(0, 0, 0));

	//particle = new Spark(L"Effect/Star.png", true);
	//particle = new Rain(L"Effect/Rain.png");
	//particle = new Snow(L"Effect/Snow.png");
	particle->Play(Vector3(0, 0, 0));

	collider = new ColliderSphere(50);
}

ParticleScene::~ParticleScene()
{
	delete particle;
	delete collider;
}

void ParticleScene::Update()
{
	particle->Update();

	if (KEY_DOWN(VK_LBUTTON))
	{
		Ray ray = CAMERA->ScreenPointToRay(mousePos);

		HitResult hitResult{};

		if (collider->Collision(ray, &hitResult))
		{
			particle->Play(hitResult.impactPoint);
		}
	}

	collider->Update();

}

void ParticleScene::Render()
{
	particle->Render();
	collider->Render();
}

void ParticleScene::PreRender()
{
}

void ParticleScene::PostRender()
{
	particle->Debug();
}
