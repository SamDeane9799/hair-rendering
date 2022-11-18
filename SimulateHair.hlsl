#include "HairGenerics.hlsli"
#include "HairPhysicsHelper.hlsli"

cbuffer HAIR_PHYSICS_CONST	: register(b0)
{
	float2x2 constraints;
	float3 force;
	float mass;
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

	//Resulting force
	float3 torque = Torque(strandInfo.Position, strandInfo.Tangent, force);

	float3 resultingForce = torque;

	float3 acceleration = AccelerationCalc(strandInfo.Acceleration, resultingForce, mass);
	float3 speed = SpeedCalc(acceleration, strandInfo.Speed, deltaTime);

	//float3 offset = 

}