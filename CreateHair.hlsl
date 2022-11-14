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
	float3 offSets[5];
	float2 UVs[5];

	ShaderVertex currentVert = vertexData[index / 3];
	float randomVariance = 1.0f + (random(UVs[cornerID]) * 0.3f - 0.15f);
	float lengthScalar = randomVariance * length;

	float3 lengthVector = normalize(currentVert.Normal) * lengthScalar;

	UVs[0] = float2(0, 0);
	UVs[1] = float2(1, 0);
	UVs[2] = float2(0, 0.66f);
	UVs[3] = float2(1, 0.66f);
	UVs[4] = float2(0.5f, 1);

	offSets[0] = float3(-width / 2.0f, 0, 0.0f);
	offSets[1] = float3(width / 2.0f, 0.0f, 0.0f);
	offSets[2] = float3(-width / 2.0f, 0, 0) + (lengthVector * 0.66f);
	offSets[3] = float3(width / 2.0f, 0, 0) + (lengthVector * 0.66f);
	offSets[4] = lengthVector;


	HairStrand newStrand;
	newStrand.Position = currentVert.Position + offSets[cornerID];
	newStrand.Normal = currentVert.Normal;
	newStrand.UV = UVs[cornerID];
	newStrand.Tangent = currentVert.Tangent;
	newStrand.padding = 0;

	hairData[index] = newStrand;
}