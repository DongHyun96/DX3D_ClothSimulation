#include "Header.hlsli"

struct VertexOutput
{
    float4 pos : SV_POSITION;
    float2 uv  : UV;
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
    float4 albedo = diffuseMap.Sample(samp, input.uv);

    float4 color;
    
    color.r = dot(albedo.rgb, float3(0.393f, 0.769f, 0.189f));
    color.g = dot(albedo.rgb, float3(0.349f, 0.646f, 0.168f));
    color.b = dot(albedo.rgb, float3(0.272f, 0.534f, 0.131f));
    color.a = 1.f;
    
    return color;
}