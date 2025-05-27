#include "LightHeader.hlsli"

struct VertexInput
{
    float4 pos       : POSITION;
    float4 baseColor : COLOR;
    float3 normal    : NORMAL;
    
    matrix transform : INSTANCE_TRANSFORM;
    float4 color     : INSTANCE_COLOR;
};

struct VertexOutput
{
    float4 pos      : SV_POSITION;
    float4 color    : COLOR;
    float3 normal   : NORMAL;
};

//cbuffer World : register(b0)
//{
//    matrix world;    
//}
//
//cbuffer View : register(b1)
//{
//    matrix view;
//    
//}
//    
//cbuffer Projection : register(b2)
//{
//    matrix proj;
//}


VertexOutput VS( VertexInput input)
{
    VertexOutput output;

    output.pos   = mul(input.pos, input.transform);
    
    output.pos   = mul(output.pos, view);
    output.pos   = mul(output.pos, proj);
    
    output.color = input.color;

    output.normal = mul(normalize(input.normal), (float3x3)input.transform);
    
	return output;
}

float4 PS( VertexOutput input) : SV_TARGET
{
    //float3 light = normalize(lights[0].direction);
    //
    //float diffuseIntensity = saturate(dot(input.normal, -light)); // N dot L
    //
    //return input.color * diffuseIntensity;
    return input.color;
}
