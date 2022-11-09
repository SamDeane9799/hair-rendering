#include "HairGenerics.hlsli"

cbuffer HAIR_PHYSICS_CONST	: register(b0)
{
	float2x2 constraints;
}
RWStructuredBuffer<HairStrand> hairData	: register(u0);

[numthreads(8, 8, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	//Figure out if we're a base vertex
	//ALTERNATIVE: Don't figure it out? with correct calculations we shouldn't be able to get out

	//Calculate all force applied as torque torque = radius * Force * sin(Angle between F and the surface
}