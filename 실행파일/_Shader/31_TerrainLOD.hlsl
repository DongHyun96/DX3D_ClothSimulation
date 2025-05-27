#include "LightHeader.hlsli"

//VS
VertexTexture VS(VertexTexture input)
{
    input.pos = mul(input.pos, world);
   
    return input;
}

//HS
struct CHullOutput
{
    float edgeTessFactor[4]     : SV_TessFactor;
    float insideTessFactor[2]   : SV_InsideTessFactor;
};

cbuffer EdgeInfo : register(b10)
{
    float minDistance;
    float maxDistance;
    float minQuality;
    float maxQuality;
}

float CalculateTessFactor(float3 pos)
{
    float d      = distance(pos, invView._41_42_43);
    float factor = saturate((d - minDistance) / (minDistance - maxDistance));
    
    return lerp(minQuality, maxQuality, factor);
}

#define NUM_CONTROL_POINTS 4

CHullOutput CHS(InputPatch<VertexTexture, NUM_CONTROL_POINTS> input)
{
    CHullOutput output;

    float3 edges[4];
    
    edges[0] = (input[0].pos + input[2].pos).xyz * 0.5f;
    edges[1] = (input[0].pos + input[1].pos).xyz * 0.5f;
    edges[2] = (input[1].pos + input[3].pos).xyz * 0.5f;
    edges[3] = (input[2].pos + input[3].pos).xyz * 0.5f;
    
    output.edgeTessFactor[0] = CalculateTessFactor(edges[0]);
    output.edgeTessFactor[1] = CalculateTessFactor(edges[1]);
    output.edgeTessFactor[2] = CalculateTessFactor(edges[2]);
    output.edgeTessFactor[3] = CalculateTessFactor(edges[3]);
    
    float3 center = (edges[0] + edges[2]) * 0.5f;
    
    output.insideTessFactor[0] = CalculateTessFactor(center);
    output.insideTessFactor[1] = CalculateTessFactor(center);

    return output;
}

struct HullOutput
{
    float4 pos : POSITION;
    float2 uv : UV;
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CHS")]
HullOutput HS(InputPatch<VertexTexture, NUM_CONTROL_POINTS> input,
                uint i : SV_OutputControlPointID)
{
    HullOutput output;

    output.pos  = input[i].pos;
    output.uv   = input[i].uv;

    return output;
}

//DS
struct DomainOutput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

Texture2D heightMap : register(t10);

cbuffer HeightBuffer : register(b10)
{
    float heightScale;
}

[domain("quad")]
DomainOutput DS(CHullOutput input, float2 domain : SV_DomainLocation,
   const OutputPatch<HullOutput, NUM_CONTROL_POINTS> patch)
{
    DomainOutput output;
    
    float4 v1 = lerp(patch[0].pos, patch[1].pos, domain.x); // domainÀº Ä­ÀÇ index °°Àº ´À³¦
    float4 v2 = lerp(patch[2].pos, patch[3].pos, domain.x);
    float4 pos = lerp(v1, v2, domain.y);
    
    float2 uv1 = lerp(patch[0].uv, patch[1].uv, domain.x);
    float2 uv2 = lerp(patch[2].uv, patch[3].uv, domain.x);
    float2 texCoord = lerp(uv1, uv2, domain.y);
    
    pos.y = heightMap.SampleLevel(samp, texCoord, 0).r * heightScale;
    
    output.pos = float4(pos.xyz, 1.0f);

    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);

    output.uv = texCoord;
    
    return output;
}

//PS
float4 PS(DomainOutput input) : SV_TARGET
{
    return diffuseMap.Sample(samp, input.uv);
}