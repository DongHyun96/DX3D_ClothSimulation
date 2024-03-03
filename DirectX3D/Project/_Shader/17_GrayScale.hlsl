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

    //float scale = (albedo.r + albedo.g + albedo.b) / 3;
    
    //float scale = albedo.r * 0.3f + albedo.g * 0.59f + albedo.b * 0.11f;
    float scale = dot(albedo.rgb, float3(0.3f, 0.59f, 0.11f)); // 위의 식과 이 식은 같음
    
    return float4(scale, scale, scale, 1.f);
}