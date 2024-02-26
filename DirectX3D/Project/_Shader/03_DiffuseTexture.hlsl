#include "Header.hlsli"
#include "LightHeader.hlsli"

struct VertexOutput
{
    float4 pos      : SV_POSITION;
    float2 uv       : UV;
    float3 normal   : NORMAL;
};


VertexOutput VS(VertexTextureNormal input)
{
    VertexOutput output;

    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.normal = mul(normalize(input.normal), (float3x3) world);
    
    output.uv = input.uv;

    return output;
}

float4 PS(VertexOutput input) : SV_TARGET
{
    float3 light = normalize(lights[0].direction);
    
    float diffuseIntensity = saturate(dot(input.normal, -light)); // N dot L
    
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    
    return albedo * diffuseIntensity;
}