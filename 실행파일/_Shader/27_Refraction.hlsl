#include "Header.hlsli"

// refractedPos : Rasterizer�� �� ��ħ -> Rasterizer���� viewPort ��ȯ�� ���� �ʾ� NDC��ǥ�� �Ѿ�� ���� / 3D ��ǥ�� PS�� �Ѿ
struct VertexOutput
{
    float4 pos          : SV_POSITION;
    float2 uv           : UV;
    float4 refractedPos : POSITION; 
};


VertexOutput VS(VertexTexture input)
{
    VertexOutput output;

    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);

    output.uv = input.uv;

    output.refractedPos = output.pos;

    return output;
}

Texture2D refractionMap         : register(t11);
Texture2D refractionNormalMap   : register(t12);

cbuffer WaterBuffer : register(b10)
{
    float4 color;
    
    float waveTime;
    float waveSpeed;
    float waveScale;
    float waveShininess;
    
    float fresnel;
}

float4 PS(VertexOutput input) : SV_TARGET
{
    float4 pos = input.refractedPos;
    
    float2 uv;
    uv.x = +pos.x / pos.w * 0.5f + 0.5f;
    uv.y = -pos.y / pos.w * 0.5f + 0.5f;
    
    input.uv += waveTime * waveSpeed;
    
    float4 normal = refractionNormalMap.Sample(samp, input.uv) * 2.f - 1.f;
    
    uv += normal.xy * waveScale; // ������ ���� ����

    return refractionMap.Sample(samp, uv) * color;

}