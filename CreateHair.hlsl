#include "HairGenerics.hlsli"
struct ShaderVertex
{
	float3 Position;	    // The position of the vertex
	float3 Normal;		// Lighting
	float3 Tangent;		// Normal mapping
	float2 UV;			// Texture mapping
	float padding;
};

RWStructuredBuffer<HairStrand> hairData	: register(u0);
StructuredBuffer<ShaderVertex> vertexData;


[numthreads(8, 8, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	int index = DTid.x;
	int cornerID = index % 3;
	float3 offSets[3];

	ShaderVertex currentVert = vertexData[index / 3];

	offSets[0] = float3(-0.1f, 0, 0.0f);
	offSets[1] = float3(0.1f, 0.0f, 0.0f);
	offSets[2] = normalize(currentVert.Normal) * .2f;

	HairStrand newStrand;
	newStrand.Position = currentVert.Position + offSets[cornerID];
	newStrand.Normal = currentVert.Normal;
	//newStrand.Position = offSets[cornerID];
	//newStrand.Normal = float3(0, 0, 0);
	newStrand.UV = float2(0, 0);

	hairData[index] = newStrand;
}