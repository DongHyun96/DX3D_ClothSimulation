#ifndef SHADER_HLSLI
#define SHADER_HLSLI

struct Vertex
{
    float4 pos : POSITION;
};

struct VertexTexture
{
    float4 pos  : POSITION;
    float2 uv   : UV;
};

struct VertexTextureNormal
{
    float4 pos      : POSITION;
    float2 uv       : UV;
    float3 normal   : NORMAL;
    
};

struct VertexColorNormal
{
    float4 pos      : POSITION;
    float4 color    : COLOR;
    float3 normal   : NORMAL;
    
};

struct VertexTextureNormalTangent
{
    float4 pos      : POSITION;
    float2 uv       : UV;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
};

struct VertexTextureNormalTangentBlend
{
    float4 pos          : POSITION;
    float2 uv           : UV;
    float3 normal       : NORMAL;
    float3 tangent      : TANGENT;
    
    int4   indices      : BLENDINDICES;
    float4 weights      : BLENDWEIGHTS;
};


//cbuffer World : register(b0)  // b0 -> 슬롯 넘버
//{
//    matrix world;
//    matrix view;
//    matrix proj;
//};

cbuffer World : register(b0)
{
    matrix world;
    int hasAnimation;
}

cbuffer View : register(b1)
{
    matrix view;
    matrix invView;
}
    
cbuffer Projection : register(b2)
{
    matrix proj;
}

// PS CBuffer
cbuffer MaterialBuffer : register(b1)
{
    float4  mDiffuse;
    float4  mSpecular;
    float4  mAmbient;
    float4  mEmissive;

    float   shininess;
    int     hasDiffuseMap;
    int     hasSpecularMap;
    int     hasNormalMap;
}

Texture2D diffuseMap  : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap   : register(t2);

SamplerState samp     : register(s0);

// Model Animator
Texture2DArray transformMap : register(t0);

struct Frame 
{
	int		clipIndex;
	uint	frameIndex;
	float	time;
	float	speed;
};
	
struct Motion 
{
	float takeTime;
	float tweenTime;
	float runningTime;
	float padding;

	Frame curFrame;
    Frame nextFrame;
};

cbuffer FrameBuffer : register(b3)
{
    Motion motion;
}

matrix SkinWorld(int4 indices, float4 weights)
{
    matrix transform = 0;
    
    matrix curAnim, nextAnim;
    
    matrix curFrame, nextFrame;

    float4 c[4], n[4];
    
    for (int i = 0; i < 4; i++)
    {
        int clipIndex[2];
        int frameIndex[2];
        float time[2];

        clipIndex[0]    = motion.curFrame.clipIndex;
        frameIndex[0]   = motion.curFrame.frameIndex;
        time[0]         = motion.curFrame.time;
        
        clipIndex[1]    = motion.nextFrame.clipIndex;
        frameIndex[1]   = motion.nextFrame.frameIndex;
        time[1]         = motion.nextFrame.time;
        
        // indices[i] -> bone의 index
        c[0] = transformMap.Load(int4(indices[i] * 4 + 0, frameIndex[0], clipIndex[0], 0));
        c[1] = transformMap.Load(int4(indices[i] * 4 + 1, frameIndex[0], clipIndex[0], 0));
        c[2] = transformMap.Load(int4(indices[i] * 4 + 2, frameIndex[0], clipIndex[0], 0));
        c[3] = transformMap.Load(int4(indices[i] * 4 + 3, frameIndex[0], clipIndex[0], 0));
        
        curFrame = matrix(c[0], c[1], c[2], c[3]);

        n[0] = transformMap.Load(int4(indices[i] * 4 + 0, frameIndex[0] + 1, clipIndex[0], 0));
        n[1] = transformMap.Load(int4(indices[i] * 4 + 1, frameIndex[0] + 1, clipIndex[0], 0));
        n[2] = transformMap.Load(int4(indices[i] * 4 + 2, frameIndex[0] + 1, clipIndex[0], 0));
        n[3] = transformMap.Load(int4(indices[i] * 4 + 3, frameIndex[0] + 1, clipIndex[0], 0));
        
        nextFrame = matrix(n[0], n[1], n[2], n[3]);

        // bone 하나의 transform / tweening
        curAnim = lerp(curFrame, nextFrame, time[0]);
        
        if (clipIndex[1] > -1) // 다음 clip이 존재
        {
            c[0] = transformMap.Load(int4(indices[i] * 4 + 0, frameIndex[1], clipIndex[1], 0));
            c[1] = transformMap.Load(int4(indices[i] * 4 + 1, frameIndex[1], clipIndex[1], 0));
            c[2] = transformMap.Load(int4(indices[i] * 4 + 2, frameIndex[1], clipIndex[1], 0));
            c[3] = transformMap.Load(int4(indices[i] * 4 + 3, frameIndex[1], clipIndex[1], 0));
        
            curFrame = matrix(c[0], c[1], c[2], c[3]);

            n[0] = transformMap.Load(int4(indices[i] * 4 + 0, frameIndex[1] + 1, clipIndex[1], 0));
            n[1] = transformMap.Load(int4(indices[i] * 4 + 1, frameIndex[1] + 1, clipIndex[1], 0));
            n[2] = transformMap.Load(int4(indices[i] * 4 + 2, frameIndex[1] + 1, clipIndex[1], 0));
            n[3] = transformMap.Load(int4(indices[i] * 4 + 3, frameIndex[1] + 1, clipIndex[1], 0));
        
            nextFrame = matrix(n[0], n[1], n[2], n[3]);

            nextAnim = lerp(curFrame, nextFrame, time[1]);

            curAnim = lerp(curAnim, nextAnim, motion.tweenTime);
        }

        transform += mul(weights[i], curAnim);
    }
    
    return transform;
}


#endif