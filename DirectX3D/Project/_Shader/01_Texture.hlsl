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

float4 PS(VertexOutput input) : SV_TARGET
{
    return diffuseMap.Sample(samp, input.uv);
}