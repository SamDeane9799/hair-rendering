#include "HairGenerics.hlsli"
#include "HairPhysicsHelper.hlsli"

cbuffer HAIR_PHYSICS_CONSTANT	: register(b0)
{
	float2x2 constraints;
	float3 force;
	float deltaTime;
}

RWStructuredBuffer<HairStrand> hairData	: register(u0);

[numthreads(8, 8, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	//Figure out if we're a base vertex
	//ALTERNATIVE: Don't figure it out? with correct calculations we shouldn't move

	//Calculate all force applied as torque torque = radius * Force * sin(Angle between F and the surface
	int index = DTid.x;
	HairStrand strandInfo = hairData[index];

	strandInfo = SimulateHair(strandInfo, force, deltaTime);

	hairData[index] = strandInfo;
}