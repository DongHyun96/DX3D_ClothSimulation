#include "LightHeader.hlsli"

struct VertexOutput // GeometryInput // VS -> GS -> RS -> PS
{
    float4 pos        : POSITION0;
    float2 size       : SIZE;
                      
    float3 viewPos    : POSITION1;
                      
    matrix view       : VIEW;
    matrix projection : PROJECTION;
};

VertexOutput VS(VertexTexture input)
{
    VertexOutput output;
    
    output.pos          = input.pos;
    output.size         = input.uv;
    output.viewPos      = invView._41_42_43;
    output.view         = view;
    output.projection   = proj;

    return output;
}

struct GeometryOutput
{
    float4 pos  : SV_POSITION; // SV�� System������ �Ѿ (���⼭�� RS��)
    float2 uv   : UV;
};

// TexCoord - uv�� ����
static const float2 TEXCOORD[4] =
{
    float2(0.f, 1.f),
    float2(0.f, 0.f),
    float2(1.f, 1.f),
    float2(1.f, 0.f)
};

// ���� �ϳ��� �޾Ƽ� 4���� ������ ����� �ѱ� ���� / GS input�� ���� �������� �迭�� �޾ƾ� ��
[maxvertexcount(4)]
void GS(point VertexOutput input[1], 
inout TriangleStream<GeometryOutput> output)
{
    // ���� ȸ���� ���� �ʰ� ī�޶� ���� �������� ������ ������ billBoard�� ������ �� ����
    float3 forward = input[0].pos.xyz - input[0].viewPos;
    forward.y = 0.0f;
    forward = normalize(forward);
    
    float3 up = float3(0, 1, 0);
    
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

float4 PS(GeometryOutput input) : SV_TARGET
{
    return diffuseMap.Sample(samp, input.uv) * mDiffuse;
}