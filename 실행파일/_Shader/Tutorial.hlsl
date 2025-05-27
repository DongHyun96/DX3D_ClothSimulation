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
    
}
    
cbuffer Projection : register(b2)
{
    matrix proj;
}


VertexOutput VS( VertexColor input )
{
    VertexOutput output;

    output.pos   = mul(input.pos, world);
    output.pos   = mul(output.pos, view);
    output.pos   = mul(output.pos, proj);
    
    output.color = input.color;

	return output;
}

float4 PS( VertexOutput input) : SV_TARGET
{
    return input.color;
}