#include "Header.hlsli"
#include "LightHeader.hlsli"

LightVertexOutput VS(VertexTextureNormalTangent input)
{
    LightVertexOutput output;
    
    output.pos = mul(input.pos, world);
    
    ////////////////////////////////////
    
    float3 cameraPos = invView._41_42_43; 
    
    output.cameraDir = normalize(output.pos.xyz - cameraPos);
    
    ////////////////////////////////////

    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.normal  = mul(normalize(input.normal), (float3x3) world);
    output.tangent = mul(normalize(input.tangent), (float3x3) world);
    
    output.binormal = cross(output.normal, output.tangent);

    output.uv = input.uv;

    return output;
}

float4 PS(LightVertexOutput input) : SV_TARGET
{
    LightMaterial material = GetLightMaterial(input);
    
    float4 ambient  = CalculateAmbient(material);
    float4 color    = CalculatePoint(material, lights[0]);
    
    return color + ambient + mEmissive;
}