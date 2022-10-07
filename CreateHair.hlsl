
struct Vertex
{
	float3 Position;	    // The position of the vertex
	float2 UV;			// Texture mapping
	float3 Normal;		// Lighting
	float3 Tangent;		// Normal mapping
};

struct HairStrand
{
	float3 Position;	    // The position of the vertex
	float2 UV;			// Texture mapping
	float3 Normal;		// Lighting
};

StructuredBuffer<Vertex> vertexData		: register(t0);
RWStructuredBuffer<HairStrand> hairData	: register(u0);

cbuffer data : register(b0)
{
	float lengthRange;
	float width;
}


[numthreads(8, 8, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	int index = DTid.x * DTid.y;
	int cornerID = index % 3;
	float3 offSets[3];


	Vertex currentVert = vertexData[index / 3];

	HairStrand hairStrand = hairData[index];

	hairStrand.Normal = currentVert.Normal;
	hairStrand.UV = currentVert.UV;

	offSets[0] = float3(-0.5f, 0, 0.0f);
	offSets[1] = float3(0.5f, 0.0f, 0.0f);
	offSets[2] = float3(0.0f, 1.0f, 0.0f);
	hairStrand.Position = currentVert.Position + offSets[cornerID];
}