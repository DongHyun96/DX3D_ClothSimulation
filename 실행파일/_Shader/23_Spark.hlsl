#include "LightHeader.hlsli"

struct VertexParticle
{
    float4 pos      : POSITION;
    float2 size     : SIZE;
    float3 velocity : VELOCITY;
};

struct VertexOutput // GeometryInput // VS -> GS -> RS -> PS
{
    float4 pos        : POSITION0;
    float2 size       : SIZE;
                      
    matrix invView    : INVVIEW;
    matrix view       : VIEW;
    matrix projection : PROJECTION;
};

cbuffer SparkBuffer : register(b10)
{
    float time;
    float duration;
}

VertexOutput VS(VertexParticle input)
{
    VertexOutput output;
    
    output.pos          = input.pos + float4(input.velocity, 0.f) * time;
    output.size         = input.size;
    output.invView      = invView;
    output.view         = view;
    output.projection   = proj;

    return output;
}

struct GeometryOutput
{
    float4 pos  : SV_POSITION; // SV는 System쪽으로 넘어감 (여기서는 RS로)
    float2 uv   : UV;
};

// TexCoord - uv와 동일
static const float2 TEXCOORD[4] =
{
    float2(0.f, 1.f),
    float2(0.f, 0.f),
    float2(1.f, 1.f),
    float2(1.f, 0.f)
};

// 정점 하나만 받아서 4개의 정점을 만들어 넘길 것임 / GS input은 단일 변수여도 배열로 받아야 함
[maxvertexcount(4)]
void GS(point VertexOutput input[1], 
inout TriangleStream<GeometryOutput> output)
{
    // 굳이 회전을 하지 않고 카메라 방향 기준으로 정점을 찍으면 billBoard를 구현할 수 있음
    float3 forward = input[0].pos.xyz - input[0].invView._41_42_43;
    forward = normalize(forward);
    
    float3 up = input[0].invView._21_22_23;
    
    float3 right = normalize(cross(up, forward));
    
    float2 halfSize = input[0].size * 0.5f;
    
    float4 vertices[4] =
    {
        float4(input[0].pos.xyz - right * halfSize.x - up * halfSize.y, 1.0f),
        float4(input[0].pos.xyz - right * halfSize.x + up * halfSize.y, 1.0f),
        float4(input[0].pos.xyz + right * halfSize.x - up * halfSize.y, 1.0f),
        float4(input[0].pos.xyz + right * halfSize.x + up * halfSize.y, 1.0f),
    };
    
    for (uint i = 0; i < 4; i++)
    {
        GeometryOutput element;
        
        element.pos = mul(vertices[i], input[0].view);
        element.pos = mul(element.pos, input[0].projection);
        
        element.uv = TEXCOORD[i];
        
        output.Append(element);
    }

}

cbuffer StartColorBuffer : register(b11)
{
    float4 startColor;
}

cbuffer EndColorBuffer : register(b12)
{
    float4 endColor;
}

float4 PS(GeometryOutput input) : SV_TARGET
{
    float4 color = lerp(startColor, endColor, time / duration);

    return diffuseMap.Sample(samp, input.uv) * color * mDiffuse;
}