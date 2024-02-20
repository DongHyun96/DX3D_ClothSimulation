#include "Header.hlsli"

struct VertexOutput
{
    float4 pos : SV_POSITION;
};

VertexOutput VS(Vertex input)
{
    VertexOutput output;

    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    return output;
}

float4 PS(VertexOutput input) : SV_TARGET
{
    return mDiffuse;
}