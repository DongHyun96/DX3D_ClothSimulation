#include "Header.hlsli"

struct VertexOutput
{
    float4 pos          : SV_POSITION;
    float4 reflectedPos : POSITION;
};


cbuffer ReflectionBuffer : register(b10)
{
    matrix reflectionView;
}

VertexOutput VS(VertexTexture input)
{
    VertexOutput output;

    output.pos = mul(input.pos, world);

    // 반사 카메라 wvp 연산
    output.reflectedPos = mul(output.pos, reflectionView);
    output.reflectedPos = mul(output.reflectedPos, proj);
    
    // 일반 카메라 wvp 연산
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);

    return output;
}

Texture2D reflectionMap : register(t10);

float4 PS(VertexOutput input) : SV_TARGET
{
    float4 pos = input.reflectedPos;
    
    float2 uv;
    uv.x = +pos.x / pos.w * 0.5f + 0.5f;
    uv.y = -pos.y / pos.w * 0.5f + 0.5f;

    return reflectionMap.Sample(samp, uv);

}