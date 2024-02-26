#include "Header.hlsli"
#include "LightHeader.hlsli"

LightVertexOutput VS(VertexTextureNormalTangentBlend input)
{
    LightVertexOutput output;
    
    matrix transform;

    [branch]
    if (hasAnimation)
        transform = mul(SkinWorld(input.indices, input.weights), world);
    else
        transform = world;
    
    output.pos = mul(input.pos, transform);
    
    output.worldPos = output.pos;

    float3 cameraPos = invView._41_42_43; // world Transform의 translation 성분

    output.viewPos = cameraPos;
    
    output.cameraDir = normalize(output.pos.xyz - cameraPos);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.normal   = mul(normalize(input.normal),  (float3x3) transform);
    output.tangent  = mul(normalize(input.tangent), (float3x3) transform);
    output.binormal = cross(output.normal, output.tangent);

    output.uv = input.uv;

    return output;
}

// Diffuse와 Reflection을 수정된 normal로 계산
float4 PS(LightVertexOutput input) : SV_TARGET
{
    LightMaterial material = GetLightMaterial(input);
    
    float4 ambient = CalculateAmbient(material);
                   
    float4 color   = CalculateLights(material);
    
    return color + ambient + mEmissive;
}