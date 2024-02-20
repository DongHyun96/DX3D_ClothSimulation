#include "Header.hlsli"

struct VertexOutput
{
    float4 pos          : SV_POSITION;
    float2 uv           : UV;
    float3 normal       : NORMAL;
    
    float3 cameraDir    : CAMERADIR;
    
    float3 tangent      : TANGENT;
    float3 binormal     : BINORMAL;
};

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
        int    clipIndex[2];  
        int   frameIndex[2]; 
        float       time[2];     

         clipIndex[0] = motion.curFrame.clipIndex;
        frameIndex[0] = motion.curFrame.frameIndex;
              time[0] = motion.curFrame.time;
        
         clipIndex[1] = motion.nextFrame.clipIndex;
        frameIndex[1] = motion.nextFrame.frameIndex;
              time[1] = motion.nextFrame.time;
        
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

VertexOutput VS(VertexTextureNormalTangentBlend input)
{
    VertexOutput output;
    
    matrix transform = mul(SkinWorld(input.indices, input.weights), world);

    output.pos = mul(input.pos, transform);
    
    ////////////////////////////////////
    
    float3 cameraPos = invView._41_42_43; // world Transform의 translation 성분
    
    output.cameraDir = normalize(output.pos.xyz - cameraPos);
    
    ////////////////////////////////////

    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.normal   = mul(normalize(input.normal), (float3x3)world);
    output.tangent  = mul(normalize(input.tangent), (float3x3)world);
    
    output.binormal = cross(output.normal, output.tangent);

    output.uv = input.uv;

    return output;
}

// Diffuse와 Reflection을 수정된 normal로 계산
float4 PS(VertexOutput input) : SV_TARGET
{
    float3 light = normalize(lightDirection);
    
    // Normal Mapping
    float4 normalMapping = normalMap.Sample(samp, input.uv); // TODO : Sample Normal Map
    
    normalMapping = normalMapping * 2.f - 1.f;
    
    float3x3 TBN = float3x3(input.tangent, input.binormal, input.normal);
    
    float3 normal = normalize(mul(normalMapping.xyz, TBN));
    
    // Diffuse Light
    float diffuseIntensity = saturate(dot(normal, -light)); // N dot L
    
    // albedo - baseColor(기본색상)
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    
    // Specular light
    float3 reflection = normalize(reflect(light, normal));

    float specularIntensity = saturate(dot(-reflection, input.cameraDir));
    
    // float shininess = 32.f; // Shininess가 클수록 intensity를 작게하여 intensity 조정
    
    specularIntensity = pow(specularIntensity, shininess);
    
    float4 diffuse = albedo * diffuseIntensity * mDiffuse;

    float4 specular = specularMap.Sample(samp, input.uv) * specularIntensity * mSpecular;
    
    // Ambient Light
    float4 ambient = albedo * mAmbient;
    
    return diffuse + specular + ambient + mEmissive; // Phong shading
}