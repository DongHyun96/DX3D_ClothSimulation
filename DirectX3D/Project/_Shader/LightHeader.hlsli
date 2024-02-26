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
    float4 emissive;
    float4 specular;

    float  shininess;
    
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
    float3 normal;
    
    return normal;
}

LightMaterial GetLightMaterial(LightVertexOutput input)
{
    LightMaterial material;
    
    return material;
}

float4 CalculateAmbient(LightMaterial material)
{
    float4 result;
    
    return result;
}

float4 CalculateDirectional(LightMaterial material, LightData data)
{
    float4 finalColor;
    
    return finalColor;
}

float4 CalculatePoint(LightMaterial material, LightData data)
{
    float4 finalColor;
    
    return finalColor;
}

float4 CalculateSpot(LightMaterial material, LightData data)
{
    float4 finalColor;
    
    return finalColor;
}

float4 CalculateCapsule(LightMaterial material, LightData data)
{
    float4 finalColor;
    
    return finalColor;
}

float4 CalculateLights(LightMaterial material)
{
    float4 color = 0;

    return color;
}