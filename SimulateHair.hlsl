
struct Vertex
{
	float3 Position;	    // The position of the vertex
	float2 UV;			// Texture mapping
	float3 Normal;		// Lighting
	float3 Tangent;		// Normal mapping
};

StructuredBuffer<Vertex> vertexData		: register(t0);
RWBuffer<uint> g_drawInstancedBuffer		: register(u0);

cbuffer data : register(b0)
{
	float lengthVariance;
	float widthVariance;
}


[numthreads(8, 8, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
}