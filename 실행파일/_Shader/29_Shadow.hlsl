#include "LightHeader.hlsli"

struct VertexOutput
{
    float4 pos      : SV_POSITION;
    float2 uv       : UV;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    
    float3  viewPos : POSITION0;
    float3 worldPos : POSITION1;
    float4  clipPos : POSITION2;
};

cbuffer LightView : register(b11)
{
    matrix lightView;
}

cbuffer LightProj : register(b12)
{
    matrix lightProj;
}

VertexOutput VS(VertexTextureNormalTangentBlend input)
{
    VertexOutput output;
    
    matrix transform;
    
    [branch]
    if (hasAnimation)
        transform = mul(SkinWorld(input.indices, input.weights), world);
    else
        transform = world;
    
    output.pos = mul(input.pos, transform);
    
    output.viewPos  = invView._41_42_43;
    output.worldPos = output.pos;
    
    // ºû ±âÁØ vp ¿¬»ê -> clipPos : ºû ±âÁØ pos
    output.clipPos = mul(output.pos,     lightView);
    output.clipPos = mul(output.clipPos, lightProj);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.uv  = input.uv;
                    
    output.normal   = normalize(mul(input.normal,  (float3x3)transform));
    output.tangent  = normalize(mul(input.tangent, (float3x3)transform));
    
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}

Texture2D depthMap : register(t10);



float4 PS(VertexOutput input) : SV_TARGET
{
    LightVertexOutput data;
    
    data.     pos = input.     pos;
    data.      uv = input.      uv;
    data.  normal = input.  normal;
    data. tangent = input. tangent;
    data.binormal = input.binormal;
    data. viewPos = input. viewPos;
    data.worldPos = input.worldPos;
    
    LightMaterial material = GetLightMaterial(data);
    
    float4 ambient = CalculateAmbient(material);
    float4   color = CalculateLights (material);
    
    float4 result = ambient + color + mEmissive;
    
    
    float2 uv = input.clipPos.xy / input.clipPos.w;
    uv.y = -uv.y;
    uv = uv * 0.5f + 0.5f;
    
    if (uv.x < 0.f || uv.x > 1.f || uv.y < 0.f || uv.y > 1.f)
        return result;
    
    float  shadowDepth = depthMap.Sample(samp, uv).r;
    
    float currentDepth = input.clipPos.z / input.clipPos.w;
    
    if (currentDepth > shadowDepth)
        return result * 0.5f;
    
    return result;
}