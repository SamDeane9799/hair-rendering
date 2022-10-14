
struct Vertex
{
	float3 Position;	    // The position of the vertex
	float3 Normal;		// Lighting
	float3 Tangent;		// Normal mapping
	float2 UV;			// Texture mapping
	float padding;
};

struct HairStrand
{
	float3 Position;	    // The position of the vertex
	float3 Normal;		// Lighting
	float2 UV;			// Texture mapping
};

StructuredBuffer<Vertex> vertexData;
RWStructuredBuffer<HairStrand> hairData	: register(u0);

cbuffer data : register(b0)
{
	float lengthRange;
	float width;
}


[numthreads(8, 8, 1)]
void main( uint DTid : SV_GroupIndex )
{
	int index = DTid;
	int cornerID = index % 3;
	float3 offSets[3];

	Vertex currentVert = vertexData[index / 3];

	hairData[index].Normal = currentVert.Normal;
	hairData[index].UV = currentVert.UV;

	offSets[0] = float3(-0.1f, 0, 0.0f);
	offSets[1] = float3(0.1f, 0.0f, 0.0f);
	offSets[2] = float3(0.0f, 0.2f, 0.0f);
	hairData[index].Position = currentVert.Position + offSets[cornerID];
}