#include "LightHeader.hlsli"

struct VertexOutput
{
    float4        pos : SV_POSITION;
    float2         uv : UV;
    float4 reflectPos : POSITION0;
    float4 refractPos : POSITION1;
    float4   worldPos : POSITION2;
};

cbuffer ReflectionBuffer : register(b10)
{
    matrix reflectionView;
}

VertexOutput VS(VertexTexture input)
{
    VertexOutput output;
    
    output.pos = mul( input.pos, world);
    
    output.worldPos = output.pos;
    
    output.reflectPos = mul(output.pos, reflectionView);
    output.reflectPos = mul(output.reflectPos, proj);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.uv = input.uv;
    output.refractPos = output.pos; 
    
    return output;
}



Texture2D reflectionMap       : register(t10);
Texture2D refractionMap       : register(t11);
Texture2D refractionNormalMap : register(t12);

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
    //Refraction
    float2 uv;
    
    uv.x = +input.refractPos.x / input.refractPos.w * 0.5f + 0.5f;
    uv.y = -input.refractPos.y / input.refractPos.w * 0.5f + 0.5f;
    
    input.uv += waveTime * waveSpeed;
    
    float4 normal = refractionNormalMap.Sample(samp, input.uv) * 2.0f - 1.0f;
    uv += normal.xy * waveScale;
    
    float4 refractionColor = refractionMap.Sample(samp, uv) * color;
    
    //Reflection 
    uv.x = +input.reflectPos.x / input.reflectPos.w * 0.5f + 0.5f;
    uv.y = -input.reflectPos.y / input.reflectPos.w * 0.5f + 0.5f;
    uv += normal.xy * waveScale;
    
    float4 reflectionColor =  reflectionMap.Sample(samp, uv);
    
    float3 viewDir = normalize(input.worldPos.xyz - invView._41_42_43);
    
    float fresnelTerm = 1 - (dot(viewDir, normal.xyz) * 1.3f);
    float4 albedo = lerp(reflectionColor, refractionColor, fresnelTerm * fresnel);
    
    // Blinn - phong
    //float3 light = normalize(lights[0].direction);
    //light.yz *= -1.0f;
    //
    //float3 halfWay = normalize(viewDir + light);
    //float specularIntensity = saturate(dot(halfWay, normal.xyz));
    //
    //specularIntensity = pow(specularIntensity, waveShininess);
    //
    //albedo = saturate(albedo + specularIntensity);
    
   return albedo * color;
}