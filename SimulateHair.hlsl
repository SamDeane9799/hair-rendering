#include "HairGenerics.hlsli"
#include "HairPhysicsHelper.hlsli"

cbuffer HAIR_PHYSICS_CONSTANT	: register(b0)
{
	float3 force;
	float deltaTime;
}

RWStructuredBuffer<HairStrand> hairData	: register(u0);

[numthreads(8, 8, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	//Figure out if we're a base vertex
	//ALTERNATIVE: Don't figure it out? with correct calculations we shouldn't move
	int index = DTid.x;
	HairStrand strandInfo = hairData[index];
	float2x3 constraints[5] = { 
		{
			strandInfo.OriginalPosition.x, strandInfo.OriginalPosition.y, strandInfo.OriginalPosition.z,
			strandInfo.OriginalPosition.x, strandInfo.OriginalPosition.y, strandInfo.OriginalPosition.z
		}, {
			strandInfo.OriginalPosition.x - 0.2f, strandInfo.OriginalPosition.y - 0.2f, strandInfo.OriginalPosition.z - 0.2f,
			strandInfo.OriginalPosition.x + 0.2f, strandInfo.OriginalPosition.y + 0.2f, strandInfo.OriginalPosition.z + 0.2f
		}, {
			strandInfo.OriginalPosition.x, strandInfo.OriginalPosition.y, strandInfo.OriginalPosition.z,
			strandInfo.OriginalPosition.x, strandInfo.OriginalPosition.y, strandInfo.OriginalPosition.z
		}, {
			strandInfo.OriginalPosition.x - 0.2f, strandInfo.OriginalPosition.y - 0.2f, strandInfo.OriginalPosition.z - 0.2f,
				strandInfo.OriginalPosition.x + 0.2f, strandInfo.OriginalPosition.y + 0.2f, strandInfo.OriginalPosition.z + 0.2f
		}, {
			strandInfo.OriginalPosition.x - 0.5f, strandInfo.OriginalPosition.y - 0.5f, strandInfo.OriginalPosition.z - 0.5f,
			strandInfo.OriginalPosition.x + 0.5f, strandInfo.OriginalPosition.y + 0.5f, strandInfo.OriginalPosition.z + 0.5f
		} 
	};

	strandInfo = SimulateHair(strandInfo, force, deltaTime, constraints[index % 5]);

	hairData[index] = strandInfo;
}