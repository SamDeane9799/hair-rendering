#include "HairGenerics.hlsli"
#include "HelperMethods.hlsli"
struct ShaderVertex
{
	float3 Position;	    // The position of the vertex
	float3 Normal;		// Lighting
	float3 Tangent;		// Normal mapping
	float2 UV;			// Texture mapping
	float padding;
};

cbuffer HAIR_CONSTANT_BUFFER	: register(b0)
{
	float length;
	float width;
}

RWStructuredBuffer<HairStrand> hairData	: register(u0);
StructuredBuffer<ShaderVertex> vertexData;


[numthreads(8, 8, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	int index = DTid.x;
	int cornerID = index % 3;
	float3 offSets[3];
	float2 UVs[3];

	UVs[0] = float2(0, 0);
	UVs[1] = float2(1, 0);
	UVs[2] = float2(0.5, 1);

	ShaderVertex currentVert = vertexData[index / 3];
	float randomVariance = 1.0f + (random(UVs[cornerID]) * 0.3f - 0.15f);
	float lengthScalar = randomVariance * length;

	offSets[0] = float3(-width/2.0f, 0, 0.0f);
	offSets[1] = float3(width/2.0f, 0.0f, 0.0f);
	offSets[2] = normalize(currentVert.Normal) * lengthScalar;


	HairStrand newStrand;
	newStrand.Position = currentVert.Position + offSets[cornerID];
	newStrand.Normal = currentVert.Normal;
	newStrand.UV = UVs[cornerID];
	newStrand.Tangent = currentVert.Tangent;
	newStrand.padding = 0;

	hairData[index] = newStrand;
}