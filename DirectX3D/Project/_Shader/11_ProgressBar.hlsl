#include "Header.hlsli"

struct VertexOutput
{
    float4 pos  : SV_POSITION;
    float2 uv   : UV;
};


VertexOutput VS(VertexTexture input)
{
    VertexOutput output;

    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.uv = input.uv;

    return output;
}

cbuffer FloatValueBuffer : register(b10)
{
    float value;
}

Texture2D backImage : register(t10);

float4 PS(VertexOutput input) : SV_TARGET
{
    if (input.uv.x < value) // front image
        return diffuseMap.Sample(samp, input.uv);

    return backImage.Sample(samp, input.uv);
}