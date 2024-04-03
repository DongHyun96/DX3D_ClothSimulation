#include "Header.hlsli"

struct LightData
{
    float3  direction;
    int     type;
    
    float4  color;
    
    float3  position;
    float   range;
    
    float   inner;
    float   outer;

    float   length;
    
    int     active;
};

struct LightMaterial
{
    float3 normal;
    float4 diffuse;
    float4 specular;

    float3 viewPos;
    float3 worldPos;
};

struct LightVertexOutput
{
    float4 pos          : SV_POSITION;
    float2 uv           : UV;
    float3 normal       : NORMAL;
    
    float3 cameraDir    : CAMERADIR;
    
    float3 tangent      : TANGENT;
    float3 binormal     : BINORMAL;

    float3 viewPos      : POSITION0;
    float3 worldPos     : POSITION1;
};

#define MAX_LIGHT 10

cbuffer LightBuffer : register(b0)
{
    LightData lights[MAX_LIGHT];
    
    int lightCount;
    
    float3 ambientLight;
    float3 ambientCeil;
}

float3 GetNormal(float3 T, float3 B, float3 N, float2 uv)
{
    T = normalize(T);
    B = normalize(B);
    N = normalize(N);
    
    float3 normal = N;

    // Normal Mapping
    [flatten]
    if (hasNormalMap)
    {
        float4 normalMapping = normalMap.Sample(samp, uv); // 0 ~ 1

        //normalMapping = normalMapping * 2.f - 1.f; // -1 ~ 1
        
        //normalMapping -= 0.5f;
        //normalMapping.z *= -1;
        //normalMapping += 0.5f;
        
        normalMapping = normalMapping * 2.f - 1.f; // -1 ~ 1
        normalMapping.z *= -1;

        float3x3 TBN = float3x3(T, B, N);
    
        normal = normalize(mul(normalMapping.xyz, TBN));
    }
    
    return normal;
}

LightMaterial GetLightMaterial(LightVertexOutput input)
{
    LightMaterial material;
    
    material.normal = GetNormal(input.tangent, input.binormal, input.normal, input.uv);

    [branch]
    if(hasDiffuseMap)
        material.diffuse = diffuseMap.Sample(samp, input.uv);
    else
        material.diffuse = float4(1, 1, 1, 1);
    
    [branch]
    if (hasSpecularMap)
        material.specular = specularMap.Sample(samp, input.uv);
    else
        material.specular = float4(0, 0, 0, 0);
    
    material.viewPos    = input.viewPos;
    material.worldPos   = input.worldPos;

    return material;
}

float4 CalculateAmbient(LightMaterial material)
{
    float up = material.normal.y * 0.5f + 0.5f; // 0 ~ 1까지 normalized

    float4 result = float4(ambientLight + up * ambientCeil, 1.f);
    
    return result * material.diffuse * mAmbient;
}

float4 CalculateDirectional(LightMaterial material, LightData data)
{
    // Blinn-Phong shading
    float3 light            = normalize(data.direction);
    
    float diffuseIntensity  = saturate(dot(material.normal, -light));
    float4 diffuse          = material.diffuse * diffuseIntensity * mDiffuse;

    float3 viewDir          = normalize(material.worldPos - material.viewPos);
    float3 halfVector       = normalize(viewDir + light);
    float specularIntnsity  = saturate(dot(-halfVector, material.normal));
    specularIntnsity        = pow(specularIntnsity, shininess);
    float4 specular         = material.specular * specularIntnsity * mSpecular;
    
    float4 finalColor       = diffuse + specular;

    return finalColor * data.color;
}

float4 CalculatePoint(LightMaterial material, LightData data)
{
    // Directional과 다르게 정점의 위치에 따른 빛의 방향을 새로 구해줌
    float3 light = material.worldPos - data.position;

    // 광원에서의 거리에 따른 빛의 세기를 조절할 것임
    float lightDistance = length(light);
    
    light = normalize(light);
    
    
    float diffuseIntensity = saturate(dot(material.normal, -light));
    
    float4 diffuse = material.diffuse * diffuseIntensity * mDiffuse;

    float3 viewDir = normalize(material.worldPos - material.viewPos);

    float3 halfVector = normalize(viewDir + light);
    
    float specularIntnsity = saturate(dot(-halfVector, material.normal));

    specularIntnsity = pow(specularIntnsity, shininess);
    
    float4 specular = material.specular * specularIntnsity * mSpecular;
    
    /////////////
    float lightDistanceNormal   = 1.f - saturate(lightDistance / data.range);
    float attention             = pow(lightDistanceNormal, 2);
    
    float4 finalColor = diffuse + specular;

    return finalColor * data.color * attention;
}

float4 CalculateSpot(LightMaterial material, LightData data)
{
    // Directional과 다르게 정점의 위치에 따른 빛의 방향을 새로 구해줌
    float3 light = material.worldPos - data.position;

    // 광원에서의 거리에 따른 빛의 세기를 조절할 것임
    float lightDistance = length(light);
    
    light = normalize(light);
    
    float diffuseIntensity = saturate(dot(material.normal, -light));
    
    float4 diffuse = material.diffuse * diffuseIntensity * mDiffuse;

    float3 viewDir = normalize(material.worldPos - material.viewPos);

    float3 halfVector = normalize(viewDir + light);
    
    float specularIntnsity = saturate(dot(-halfVector, material.normal));

    specularIntnsity = pow(specularIntnsity, shininess);
    
    float4 specular = material.specular * specularIntnsity * mSpecular;
    
    /////////////
    
    float3 dir = normalize(data.direction);
    
    float cosAngle = dot(dir, light);
    
    float outer = cos(radians(data.outer));
    float inner = 1.f / cos(radians(data.inner));
    
    float conAttention = saturate((cosAngle - outer) * inner);
    conAttention       = pow(conAttention, 2);
    
    float lightDistanceNormal = 1.f - saturate(lightDistance / data.range);
    
    float attention = pow(lightDistanceNormal, 2);
    
    float4 finalColor = diffuse + specular;

    return finalColor * data.color * attention * conAttention;
}

float4 CalculateCapsule(LightMaterial material, LightData data)
{
    float3 dir = normalize(data.direction);
    
    float3 start = material.worldPos - data.position;

    // length로 나눈 것은 normalize가 아닌 범위 제한을 하기 위함
    float distanceOnLine    = dot(start, dir) / data.length; 
    distanceOnLine          = saturate(distanceOnLine) * data.length;

    float3 pointOnLine = data.position + dir * distanceOnLine;

    float3 light = material.worldPos - pointOnLine;

    float lightDistance = length(light);
    
    light = normalize(light);
    
    float diffuseIntensity = saturate(dot(material.normal, -light));
    
    float4 diffuse = material.diffuse * diffuseIntensity * mDiffuse;

    float3 viewDir = normalize(material.worldPos - material.viewPos);

    float3 halfVector = normalize(viewDir + light);
    
    float specularIntnsity = saturate(dot(-halfVector, material.normal));

    specularIntnsity = pow(specularIntnsity, shininess);
    
    float4 specular = material.specular * specularIntnsity * mSpecular;
    
    /////////////
    float lightDistanceNormal   = 1.f - saturate(lightDistance / data.range);
    float attention             = pow(lightDistanceNormal, 2);
    
    float4 finalColor = diffuse + specular;

    return finalColor * data.color * attention;
}

float4 CalculateLights(LightMaterial material)
{
    float4 color = 0;

    //[unroll]
    [unroll(MAX_LIGHT)]
    for (int i = 0; i < lightCount; i++)
    {
        [flatten]
        if (!lights[i].active)
            continue;
        
        [branch]
        if (lights[i].type == 0)                                // directional
            color += CalculateDirectional(material, lights[i]);
        else if (lights[i].type == 1)                           // Point
            color += CalculatePoint(material, lights[i]);
        else if (lights[i].type == 2)                           // SpotLight
            color += CalculateSpot(material, lights[i]);
        else if (lights[i].type == 3)                           // Capsule
            color += CalculateCapsule(material, lights[i]);
        
    }

    return color;
}