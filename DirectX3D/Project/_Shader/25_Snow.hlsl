#include "LightHeader.hlsli"

struct VertexInput
{
    float4 pos    : POSITION;
    float2 size   : SIZE;
    float2 random : RANDOM;
};

struct VertexOutput
{
   float3 pos        : POSITION0;
   float2 size       : SIZE;
   matrix invView    : INVVIEW;
   matrix view       : VIEW;
   matrix projection : PROJECTION;
   float  distance   : DISTANCE;
   float4 color      : COLOR;
   float3 velocity   : VELOCITY;
};

struct GeometryOutput
{
   float4 pos   : SV_POSITION;
   float2 uv    : UV;
   float4 color : COLOR;
};

cbuffer RainBuffer : register(b10)
{
   float3 velocity;
   float  drawDistance;
   
   float4 color;
   
   float3 origin;
   float  time;
   
   float3 size;
   float  turbulence;
}

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    
    float3 displace = time * velocity;
    
    input.pos.x += cos(time - input.random.x) * turbulence;
    input.pos.z += cos(time - input.random.y) * turbulence;
    
    input.pos.xyz = origin + (size + (input.pos.xyz + displace) % size) % size - (size * 0.5f);

    output.velocity = velocity;
    output.distance = drawDistance;
    output.color    = color;
    
    output.pos        = input.pos;
    output.size       = input.size;
    output.invView    = invView;
    output.view       = view;
    output.projection = proj;
    
    return output;
}

static const float2 TEXCOORD[4] =
{
   float2(0.0f, 1.0f),
   float2(0.0f, 0.0f),
   float2(1.0f, 1.0f),
   float2(1.0f, 0.0f)
};

[maxvertexcount(4)]
void GS(point VertexOutput input[1], inout TriangleStream<GeometryOutput> output)
{
   float3 up = normalize(-input[0].velocity);
   float3 forward = normalize(input[0].pos.xyz - input[0].invView._41_42_43);
   float3 right = normalize(cross(up, forward));
   
   float2 halfSize = input[0].size * 0.5f;
   
   float4 vertices[4];
   vertices[0] = float4(input[0].pos.xyz - right * halfSize.x - up * halfSize.y, 1.0f);
   vertices[1] = float4(input[0].pos.xyz - right * halfSize.x + up * halfSize.y, 1.0f);
   vertices[2] = float4(input[0].pos.xyz + right * halfSize.x - up * halfSize.y, 1.0f);
   vertices[3] = float4(input[0].pos.xyz + right * halfSize.x + up * halfSize.y, 1.0f);
   
   GeometryOutput element;
   element.color = input[0].color;
   
   [unroll(4)]
   for (uint i = 0; i < 4; i++)
   {
      element.pos = mul(vertices[i], input[0].view);
      element.pos = mul(element.pos, input[0].projection);
      
      element.uv = TEXCOORD[i];
      
        element.color.a = 0.2f * saturate(1 - element.pos.z / input[0].distance)
         * input[0].color.a;
      
      output.Append(element);
   }
}

float4 PS(GeometryOutput input) : SV_TARGET
{
   return diffuseMap.Sample(samp, input.uv) * input.color;
}