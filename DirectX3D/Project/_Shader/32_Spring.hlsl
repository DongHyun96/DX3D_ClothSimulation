#include "Header.hlsli"

cbuffer SpringBuffer : register(b20)
{
    float3 startPoint;
    float3 endPoint;
}

struct VertexColor
{
    float4 pos      : POSITION;
    float4 color    : COLOR;
};

struct VertexOutput
{
    float4 pos      : SV_POSITION;
    float4 color    : COLOR;
};


VertexOutput VS(VertexColor input)
{
    VertexOutput output;

    float3 direction = normalize(endPoint - startPoint);
    
    float3 newPosition = input.pos + startPoint;
    
    float3 up       = float3(0, 1, 0);
    float3 axis     = normalize(cross(up, direction));
    float  angle    = acos(dot(up, direction));
    float3 position = mul(rotate(angle, axis), newPosition - startPoint) + startPoint;

    output.pos      = mul(input.pos, world);
    output.pos      = mul(output.pos, view);
    output.pos      = mul(output.pos, proj);

    output.color    = input.color;

    return output;
}

float4 PS(VertexOutput input) : SV_TARGET
{
    return float4(1, 0, 0, 1);
}
