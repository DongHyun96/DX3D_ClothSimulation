#include "Header.hlsli"

struct VertexOutput
{
    float4 pos      : SV_POSITION;
    float4 localPos : LOCALPOS;
};


VertexOutput VS(VertexTexture input)
{
    VertexOutput output;

    //output.pos = mul(input.pos, view);

    output.pos.xyz = mul(input.pos.xyz, (float3x3) view);
    output.pos.w = 1.f;
    
    output.pos = mul(output.pos, proj);

    output.localPos = input.pos;
    
    return output;
}

TextureCube cubeMap : register(t10);

float4 PS(VertexOutput input) : SV_TARGET
{
    return cubeMap.Sample(samp, input.localPos.xyz);
}