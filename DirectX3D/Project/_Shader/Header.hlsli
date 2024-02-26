#ifndef SHADER_HLSLI
#define SHADER_HLSLI

struct Vertex
{
    float4 pos : POSITION;
};

struct VertexTexture
{
    float4 pos  : POSITION;
    float2 uv   : UV;
};

struct VertexTextureNormal
{
    float4 pos      : POSITION;
    float2 uv       : UV;
    float3 normal   : NORMAL;
    
};

struct VertexColorNormal
{
    float4 pos      : POSITION;
    float4 color    : COLOR;
    float3 normal   : NORMAL;
    
};

struct VertexTextureNormalTangent
{
    float4 pos      : POSITION;
    float2 uv       : UV;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
};

struct VertexTextureNormalTangentBlend
{
    float4 pos          : POSITION;
    float2 uv           : UV;
    float3 normal       : NORMAL;
    float3 tangent      : TANGENT;
    
    int4   indices      : BLENDINDICES;
    float4 weights      : BLENDWEIGHTS;
};


//cbuffer World : register(b0)  // b0 -> ½½·Ô ³Ñ¹ö
//{
//    matrix world;
//    matrix view;
//    matrix proj;
//};

cbuffer World : register(b0)
{
    matrix world;
}

cbuffer View : register(b1)
{
    matrix view;
    matrix invView;
}
    
cbuffer Projection : register(b2)
{
    matrix proj;
}



cbuffer MaterialBuffer : register(b1)
{
    float4  mDiffuse;
    float4  mSpecular;
    float4  mAmbient;
    float4  mEmissive;

    float   shininess;
    int     hasDiffuseMap;
    int     hasSpecularMap;
    int     hasNormalMap;
}

Texture2D diffuseMap  : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap   : register(t2);

SamplerState samp     : register(s0);

#endif