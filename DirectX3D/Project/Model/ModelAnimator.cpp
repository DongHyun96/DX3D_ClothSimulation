#include "Framework.h"
#include "ModelAnimator.h"


ModelAnimator::ModelAnimator(string name)
	:Model(name)
{
    buffer = new FrameBuffer;
    
    SetShader(L"09_ModelAnimator");
}

ModelAnimator::~ModelAnimator()
{
    for (ModelClip* clip : clips)
        delete clip;

    clips.clear();

    delete buffer;

    srv->Release();
    texture->Release();
}

void ModelAnimator::Update()
{
    Model::Update();

    if (isPlay) UpdateFrame();
}

void ModelAnimator::Render()
{
    if (!texture) CreateTexture();

    buffer->SetVSBuffer(3); // 0,1,2 wvp

    DC->VSSetShaderResources(0, 1, &srv); // 클립의 Transform 정보를 srv형식으로 넘김

    Model::Render(true);
}

void ModelAnimator::Debug()
{
    static int clipIndex = 0;

    ImGui::Text("ModelAnimator Option");

    ImGui::Checkbox("IsPlay", &isPlay);

    if (ImGui::SliderInt("ClipIndex", &clipIndex, 0, clips.size() - 1))
    {
        //PlayClip(clipIndex, 0.1f, 2.f);
        PlayClip(clipIndex);
    }

    ImGui::SliderInt("FrameIndex", (int*)&buffer->data.curFrame.frameIndex, 0, clips[clipIndex]->frameCount - 1);

    ImGui::SliderFloat("Clip Speed", (float*)&buffer->data.curFrame.speed, 0.1f, 10.f);

    ImGui::SliderFloat("TakeTime", (float*)&buffer->data.takeTime, 0.1f, 10.f);

    Model::Debug();
}

void ModelAnimator::ReadClip(string file, UINT clipIndex)
{
	// .clip 불러오기

    string path = "_ModelData/Clip/" + name + "/" + file + "_" + to_string(clipIndex) + ".clip";

    BinaryReader data(path);

    ModelClip* clip = new ModelClip();
    clip->name              = data.ReadString();
    clip->ticksPerSecond    = data.ReadFloat();
    clip->frameCount        = data.ReadUint();
    //clip->duration        = data.ReadFloat();

    UINT keyFrameCount = data.ReadUint();

    for (UINT i = 0; i < keyFrameCount; i++)
    {
        KeyFrame* keyFrame = new KeyFrame;
        keyFrame->boneName = data.ReadString();

        UINT keyTransformCount = data.ReadUint();

        if (keyTransformCount > 0)
        {
            keyFrame->transforms.resize(keyTransformCount);

            void* ptr = (void*)keyFrame->transforms.data();
            data.ReadData(&ptr, sizeof(KeyTransform) * keyTransformCount);
        }

        clip->keyFrames.emplace(keyFrame->boneName, keyFrame);
    }

    clips.emplace_back(clip);
}

void ModelAnimator::PlayClip(int clipIndex, float speed, float takeTime)
{
    clips[buffer->data.curFrame.clipIndex]->Init();
    clips[clipIndex]->Init();

    buffer->data.nextFrame.clipIndex = clipIndex;
    buffer->data.nextFrame.speed     = speed;
    buffer->data.takeTime            = takeTime;

    isPlay = true;
}

void ModelAnimator::CreateTexture()
{
    UINT clipCount = clips.size();

    clipTransforms = new ClipTransform[clipCount];
    nodeTransforms = new ClipTransform[clipCount];

    for (UINT i = 0; i < clipCount; i++)
    {
        CreateClipTransform(i);
    }

    D3D11_TEXTURE2D_DESC desc = {};

    desc.Width              = MAX_BONE * 4; // 4*4 행렬을 만들기 위함
    desc.Height             = MAX_FRAME;
    desc.ArraySize          = clipCount;
    desc.Format             = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.Usage              = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
    desc.MipLevels          = 1;
    desc.SampleDesc.Count   = 1;

    UINT pageSize = MAX_BONE * sizeof(Matrix) * MAX_FRAME;

    void* ptr = VirtualAlloc(nullptr, pageSize * clipCount, MEM_RESERVE, PAGE_READWRITE);
    
    for (UINT c = 0; c < clipCount; c++)
    {
        UINT start = c * pageSize;

        for (UINT i = 0; i < MAX_FRAME; i++)
        {
            void* temp = (BYTE*)ptr + MAX_BONE * i * sizeof(Matrix) + start;

            VirtualAlloc(temp, MAX_BONE * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);
            memcpy(temp, clipTransforms[c].transform[i], MAX_BONE * sizeof(Matrix));
        }
    }

    D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[clipCount];

    for (UINT c = 0; c < clipCount; c++)
    {
        void* temp = (BYTE*)ptr + c * pageSize;

        subResource[c].pSysMem              = temp;
        subResource[c].SysMemPitch          = MAX_BONE * sizeof(Matrix);
        subResource[c].SysMemSlicePitch     = pageSize;
    }

    DEVICE->CreateTexture2D(&desc, subResource, &texture);

    delete[] subResource;
    VirtualFree(ptr, 0, MEM_RELEASE); // 0 ?

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

    srvDesc.Format                      = DXGI_FORMAT_R32G32B32A32_FLOAT;
    srvDesc.ViewDimension               = D3D11_SRV_DIMENSION_TEXTURE2DARRAY; // 2D Texture를 배열 형식으로 넘김
    srvDesc.Texture2DArray.MipLevels    = 1;
    srvDesc.Texture2DArray.ArraySize    = clipCount;

    DEVICE->CreateShaderResourceView(texture, &srvDesc, &srv);
}

Matrix ModelAnimator::GetTransformByBone(string boneName)
{
    if (!texture) return XMMatrixIdentity();
    
    UINT clipIndex  = buffer->data.curFrame.clipIndex;
    UINT frameIndex = buffer->data.curFrame.frameIndex;
    float time      = buffer->data.curFrame.time;

    Matrix curTransform{}, nextTransform{};

    int nodeIndex = -1;

    for (NodeData node : nodes)
    {
        if (node.name == boneName)
            nodeIndex = node.index;
    }

    if (nodeIndex == -1)
        return XMMatrixIdentity();

    curTransform  = nodeTransforms[clipIndex].transform[frameIndex + 0][nodeIndex];
    nextTransform = nodeTransforms[clipIndex].transform[frameIndex + 1][nodeIndex];

    Matrix curAnim = LERP(curTransform, nextTransform, time);

    clipIndex   = buffer->data.nextFrame.clipIndex;
    frameIndex  = buffer->data.nextFrame.frameIndex;
    time        = buffer->data.nextFrame.time;

    if (clipIndex == -1) return curAnim;

    curTransform  = nodeTransforms[clipIndex].transform[frameIndex + 0][nodeIndex];
    nextTransform = nodeTransforms[clipIndex].transform[frameIndex + 1][nodeIndex];

    Matrix nextAnim = LERP(curTransform, nextTransform, time);

    return LERP(curAnim, nextAnim, buffer->data.tweenTime);
}

void ModelAnimator::CreateClipTransform(UINT clipIndex)
{
    // S, R, T 따로 가져오지 않고 transform으로 계산된 값을 가져올 것임
    ModelClip* clip = clips[clipIndex];

    for (UINT f = 0; f < clip->frameCount; f++)
    {
        UINT nodeIndex = 0;

        for (NodeData node : nodes)
        {
            Matrix animWorld;

            KeyFrame* frame = clip->GetKeyFrames(node.name);

            if (frame != nullptr)
            {
                KeyTransform transform = frame->transforms[f];

                Matrix S = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
                Matrix R = XMMatrixRotationQuaternion(XMLoadFloat4(&transform.rotation));
                Matrix T = XMMatrixTranslation(transform.translation.x, transform.translation.y, transform.translation.z);

                animWorld = S * R * T; // Local World
            }
            else
            {
                animWorld = XMMatrixIdentity();
            }

            Matrix parentWorld;

            int parentIndex = node.parent;

            if (parentIndex < 0)
                parentWorld = XMMatrixIdentity();
            else
                parentWorld = nodeTransforms[clipIndex].transform[f][parentIndex];

            nodeTransforms[clipIndex].transform[f][nodeIndex] = animWorld * parentWorld; // Local world to Global world

            if (boneMap.count(node.name) > 0)
            {
                UINT boneIndex = boneMap[node.name];

                Matrix transform = bones[boneIndex].offset;

                transform *= nodeTransforms[clipIndex].transform[f][nodeIndex];

                clipTransforms[clipIndex].transform[f][boneIndex] = transform;
            }

            nodeIndex++;
        }
    }
}

void ModelAnimator::UpdateFrame()
{
    Motion& motion = buffer->data;
    
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

