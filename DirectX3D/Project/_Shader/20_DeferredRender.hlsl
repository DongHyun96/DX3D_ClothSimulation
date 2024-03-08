#include "LightHeader.hlsli"

struct VertexOutput
{
    float4 pos  : SV_POSITION;
    float2 uv   : UV;
};

// 화면 기준 base 좌표
static const float2 arrBasePos[4] =
{
    float2(-1.f, +1.f),
    float2(+1.f, +1.f),
    float2(-1.f, -1.f),
    float2(+1.f, -1.f),
};

VertexOutput VS(uint vertexID : SV_VertexID)
{
    VertexOutput output;
    output.pos = float4(arrBasePos[vertexID].xy, 0.f, 1.f);
    
    output.uv = output.pos.xy;

    return output;
}

struct SurfaceData
{
    float   linearDepth;
    float3  color;
    float3  normal;
    float   specInt; // spectrum
    float   specPow;
};

Texture2D diffuseTex    : register(t10);
Texture2D specularTex   : register(t11);
Texture2D normalTex     : register(t12);
Texture2D depthTex      : register(t13);

float ConvertDepthToLinear(float depth)
{
    float linearDepth = proj._43 / (depth - proj._33);
    
    return linearDepth;
}

SurfaceData UnpackGBuffer(int2 location)
{
    SurfaceData output;
    
    int3 location3 = int3(location, 0);
    
    float depth = depthTex.Load(location3).x;

    output.linearDepth = ConvertDepthToLinear(depth);

    output.color = diffuseTex.Load(location3).rgb;
    
    float4 specular = specularTex.Load(location3);
    output.specPow  = MIN_SHININESS + specular.x * MAX_SHININESS;
    output.specInt  = specular.y; // x에는 pow, y에는 intensity 를 저장했었음
    
    output.normal = normalTex.Load(location3).xyz;
    output.normal = normalize(output.normal * 2.f - 1.f);
    
    return output;
}

float3 CalculateWorldPos(float2 pos, float linearDepth)
{
    float4 position;
    
    float2 temp;
    temp.x = 1 / proj._11;
    temp.y = 1 / proj._22;

    position.xy = pos.xy * temp * linearDepth;
    position.z = linearDepth;
    position.w = 1.f;
    
    return mul(position, invView).xyz;
}

float4 PS(VertexOutput input) : SV_TARGET
{
    SurfaceData data = UnpackGBuffer(input.pos.xy); // pos.xy = input.uv
    
    LightMaterial material;
    
    material.normal     = data.normal;
    material.diffuse    = float4(data.color, 1.f);
    material.specular   = data.specInt;
    material.viewPos    = invView._41_42_43;
    material.worldPos   = CalculateWorldPos(input.uv, data.linearDepth);

    return CalculateLights(material);
}