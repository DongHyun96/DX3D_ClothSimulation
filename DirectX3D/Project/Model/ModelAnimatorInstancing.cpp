#include "Framework.h"
#include "ModelAnimatorInstancing.h"


ModelAnimatorInstancing::ModelAnimatorInstancing(string name)
	:ModelAnimator(name)
{
	instanceDatas.resize(MAX_INSTANCE);

	instanceBuffer = new VertexBuffer(instanceDatas);

	SetShader(L"14_ModelAnimatorInstancing");

    buffer = new FrameInstancingBuffer;
}

ModelAnimatorInstancing::~ModelAnimatorInstancing()
{
	delete instanceBuffer;

	for (Transform* transform : transforms)
		delete transform;

	transforms.clear();

	delete buffer;
}

void ModelAnimatorInstancing::Update()
{
	drawCount = 0;

	for (UINT i = 0; i < transforms.size(); i++)
	{
		if (!transforms[i]->GetIsActive()) continue;

		transforms[i]->Update();

		instanceDatas[drawCount].world = XMMatrixTranspose(transforms[i]->GetWorld());
		instanceDatas[drawCount].index = i;

		drawCount++;

		UpdateFrame(i);
	}

	instanceBuffer->UpdateVertex(instanceDatas.data(), drawCount);
}

void ModelAnimatorInstancing::Render()
{
    if (!texture) CreateTexture();

    buffer->SetVSBuffer(3); // 0,1,2 wvp

    DC->VSSetShaderResources(0, 1, &srv); // 클립의 Transform 정보를 srv형식으로 넘김

	instanceBuffer->IASetBuffer(1);

    buffer->SetVSBuffer(3);

	__super::RenderInstanced(drawCount);
}

void ModelAnimatorInstancing::Debug()
{
	ImGui::Text("DrawCount : %d", drawCount);

	if (ImGui::Button("Add Model"))
		AddTransform();

    ImGui::SliderInt("InstanceIndex", &instanceIndex, 0, 127);
    ImGui::SliderInt("ClipIndex", &clipIndex, 0, clips.size() - 1);

    if (ImGui::Button("Play"))
        PlayClip(instanceIndex, clipIndex);

	for (Transform* transform : transforms)
		transform->Debug();
}

Transform* ModelAnimatorInstancing::AddTransform()
{
	Transform* transform = new Transform;

	transform->SetName(name + "_" + to_string(transforms.size()));

	transforms.emplace_back(transform);

	return transform;
}

void ModelAnimatorInstancing::PlayClip(UINT instanceIndex, int clipIndex, float speed, float takeTime)
{
    Motion& motion = buffer->data.motion[instanceIndex];

    clips[motion.curFrame.clipIndex]->Init();
    clips[clipIndex]->Init();

    motion.nextFrame.clipIndex = clipIndex;
    motion.nextFrame.speed     = speed;
    motion.takeTime            = takeTime;

    isPlay = true;
}

void ModelAnimatorInstancing::UpdateFrame(UINT instanceIndex)
{
	Motion& motion = buffer->data.motion[instanceIndex];
    
    ModelClip* clip = clips[motion.curFrame.clipIndex];

    motion.curFrame.time += DELTA_TIME * clip->ticksPerSecond * motion.curFrame.speed;

    if (motion.curFrame.time >= 1.f) // 1 프레임이 지났음
    {
        if (motion.curFrame.frameIndex == 0)
            clip->Init(); // clip 콜백 map iterator begin으로 초기화

        float animRatio = (float)motion.curFrame.frameIndex / clip->frameCount;

        clip->Execute(animRatio); // 비율에 맞는 함수 포인터가 있으면 callBack

        motion.curFrame.frameIndex = (motion.curFrame.frameIndex + 1) % (clip->frameCount - 1); // 마지막 프레임은 쓰지 않음 (Tweening때문에 수정)
        motion.curFrame.time = 0.f;
    }

    if (motion.nextFrame.clipIndex == -1) return;

    motion.tweenTime += DELTA_TIME / motion.takeTime;

    clip = clips[motion.nextFrame.clipIndex];

    if (motion.tweenTime >= 1.f) // 다음 motion으로 전환
    {
        motion.curFrame             = motion.nextFrame;
        motion.tweenTime            = 0.f;

        motion.nextFrame.clipIndex  = -1;
        motion.nextFrame.frameIndex = 0;
        motion.nextFrame.time       = 0.f;
    }
    else // Tweening 하는 순간
    {
        motion.nextFrame.time += DELTA_TIME * clip->ticksPerSecond * motion.nextFrame.speed;

        if (motion.nextFrame.time >= 1.f)
        {
            motion.nextFrame.frameIndex = (motion.nextFrame.frameIndex + 1) % (clip->frameCount - 1);
            motion.nextFrame.time = 0.f;
        }
        
    }
}
