#include "Header.hlsli"
#include "LightHeader.hlsli"


struct VertexOutput
{
    float4 pos          : SV_POSITION;
    float2 uv           : UV;
    float3 normal       : NORMAL;
    
    float3 cameraDir    : CAMERADIR;
    
    float3 worldPos     : WORLDPOS;
    
    float3 tangent      : TANGENT;
    float3 binormal     : BINORMAL;
};


VertexOutput VS(VertexTextureNormalTangent input)
{
    VertexOutput output;
    
    output.pos = mul(input.pos, world);
    
    output.worldPos = output.pos;
    
    float3 cameraPos = invView._41_42_43; // world Transform의 translation 성분
    
    output.cameraDir = normalize(output.pos.xyz - cameraPos);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.normal = mul(normalize(input.normal), (float3x3) world);
    output.tangent = mul(normalize(input.tangent), (float3x3) world);

    output.binormal = cross(output.normal, output.tangent);
    
    output.uv = input.uv;

    return output;
}

Texture2D alphaMap          : register(t10);
Texture2D secondDiffuseMap  : register(t11);


float4 PS(VertexOutput input) : SV_TARGET
{
    float3 light = normalize(lights[0].direction);
    
    // Normal Mapping
    float4 normalMapping = normalMap.Sample(samp, input.uv);
    
    normalMapping = normalMapping * 2.f - 1.f;
    
    float3x3 TBN = float3x3(input.tangent, input.binormal, input.normal);
    
    float3 normal = normalize(mul(normalMapping.xyz, TBN));
    
    // Diffuse Light
    float diffuseIntensity = saturate(dot(normal, -light)); // N dot L
    
    // albedo - baseColor(기본색상)
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    
    // Texture Splatting

    float4 alpha = alphaMap.Sample(samp, input.uv);

    float4 secondAlbedo = secondDiffuseMap.Sample(samp, input.uv);

    albedo = lerp(albedo, secondAlbedo, alpha.r);
    
    // Specular light
    float3 reflection = normalize(reflect(light, normal));

    float specularIntensity = saturate(dot(-reflection, input.cameraDir));
    
    float shininess = 32.f; // Shininess가 클수록 intensity를 작게하여 intensity 조정
    
    specularIntensity = pow(specularIntensity, shininess);
    
    float4 diffuse = albedo * diffuseIntensity;

    float4 specular = specularMap.Sample(samp, input.uv) * specularIntensity;
    
    // Ambient Light
    float4 ambient = albedo * float4(0.1f, 0.1f, 0.1f, 1.f);
    
    return diffuse + specular + ambient;
}