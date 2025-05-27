cbuffer RayBuffer : register(b0)
{
    float3  origin;
    uint    polygonCount;
    
    float3  direction;
}

struct InputDesc
{
    float3 v0, v1, v2;
};

StructuredBuffer<InputDesc> input : register(t0); // SRV

struct OutputDesc
{
    int    isPicked;

    float  distance;
};

RWStructuredBuffer<OutputDesc> output : register(u0);

void Intersection(uint index)
{
    float3 A = input[index].v0;
    float3 B = input[index].v1;
    float3 C = input[index].v2;
    
    float3 e0 = B - A;
    float3 e1 = C - A;
    
    float3 P, T, Q;
    
    P = cross(direction, e1);
    float d = 1.f / dot(e0, P);
    
    float u, v;
    
    T = origin - A;
    u = dot(T, P) * d;
    
    Q = cross(T, e0);
    v = dot(direction, Q) * d;
    output[index].distance = dot(e1, Q) * d;
    
    bool b = (u >= 0.f) && (v >= 0.f) && (u + v < 1.f) && (output[index].distance >= 0.f);
    
    output[index].isPicked = b;
}

[numthreads(32, 32, 1)]
void CS( uint3 groupID : SV_GroupID, uint groupIndex : SV_GroupIndex )
{
    uint index = groupID * 32 * 32 + groupIndex;
    
    if (polygonCount > index)
        Intersection(index);
}
