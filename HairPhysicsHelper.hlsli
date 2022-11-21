// Include guard
#ifndef _HAIRPHYSICSHELPER_HLSL
#define _HAIRPHYSICSHELPER_HLSL
#define HAIR_WEIGHT 0.5f
/*
* a = lever arm
* b = vector connected to lever arm
* F = Force vector being applied
*/
float3 Torque(float3 a, float3 b, float3 F) {
	float theta = acos(dot(a, b) / abs(a) * abs(b));
	float radius = length(a);

	float3 torque = radius * F * (sin(theta));
	return torque;
}

/*
* currentAccel = Unit's current acceleration
* force = Force being applied to object in 3 dimnesions
* mass = mass of object in kg
*/
float3 AccelerationCalc(float3 currentAccel, float3 force, float mass) {
	return currentAccel + (force * mass);
}

/*
* acceleration = current acceleration of object
* currentSpeed = current speed of object
* deltaTime = time since last frame
*/
float3 SpeedCalc(float3 acceleration, float3 currentSpeed, float deltaTime) {
	return currentSpeed + float3(acceleration * deltaTime);
}

/*
* currentPosition = new position of vertex
* speed = resulting speed this frame of object
* deltaTime = time since last frame
*/
float3 PositionCalc(float3 currentPosition, float3 speed, float deltaTime) {
	return currentPosition + (speed * deltaTime);
}

HairStrand SimulateHair(HairStrand strand, float3 force, float deltaTime)
{
	float3 torque = Torque(strand.Position, strand.Tangent, force);

	float3 acceleration = AccelerationCalc(strand.Acceleration, torque, HAIR_WEIGHT);
	float3 speed = SpeedCalc(strand.Acceleration, strand.Speed, deltaTime);

	strand.Speed = speed;
	strand.Acceleration = acceleration;
	strand.Position = strand.Position + (speed * deltaTime);
	return strand;
}
#endif