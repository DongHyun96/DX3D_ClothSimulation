#include "Header.hlsli"
#include "LightHeader.hlsli"

struct VertexOutput
{
    float4 pos          : SV_POSITION;
    float2 uv           : UV;
    float3 normal       : NORMAL;
    
    float3 cameraDir    : CAMERADIR;
    
    float3 tangent      : TANGENT;
    float3 binormal     : BINORMAL;
};


VertexOutput VS(VertexTextureNormalTangent input)
{
    VertexOutput output;
    
    output.pos = mul(input.pos, world);
    
    ////////////////////////////////////
    
    float3 cameraPos = invView._41_42_43; // world Transform의 translation 성분
    
    output.cameraDir = normalize(output.pos.xyz - cameraPos);
    
    ////////////////////////////////////

    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.normal   = mul(normalize(input.normal), (float3x3)world);
    output.tangent  = mul(normalize(input.tangent), (float3x3)world);
    
    output.binormal = cross(output.normal, output.tangent);

    output.uv = input.uv;

    return output;
}

// Diffuse와 Reflection을 수정된 normal로 계산
float4 PS(VertexOutput input) : SV_TARGET
{
    float3 light = normalize(lights[0].direction);
    
    // Normal Mapping
    float4 normalMapping = normalMap.Sample(samp, input.uv); // TODO : Sample Normal Map
    
    normalMapping = normalMapping * 2.f - 1.f;
    
    float3x3 TBN = float3x3(input.tangent, input.binormal, input.normal);
    
    float3 normal = normalize(mul(normalMapping.xyz, TBN));
    
    // Diffuse Light
    float diffuseIntensity = saturate(dot(normal, -light)); // N dot L
    
    // albedo - baseColor(기본색상)
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    
    // Specular light
    float3 reflection = normalize(reflect(light, normal));

    float specularIntensity = saturate(dot(-reflection, input.cameraDir));
    
    // float shininess = 32.f; // Shininess가 클수록 intensity를 작게하여 intensity 조정
    
    specularIntensity = pow(specularIntensity, shininess);
    
    float4 diffuse = albedo * diffuseIntensity * mDiffuse;

    float4 specular = specularMap.Sample(samp, input.uv) * specularIntensity * mSpecular;
    
    // Ambient Light
    float4 ambient = albedo * mAmbient;
    
    return diffuse + specular + ambient + mEmissive; // Phong shading
}