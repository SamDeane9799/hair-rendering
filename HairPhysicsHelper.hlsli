// Include guard
#ifndef _HAIRPHYSICSHELPER_HLSL
#define _HAIRPHYSICSHELPER_HLSL
/*
* a = lever arm
* b = vector connected to lever arm
* F = Force vector being applied
*/
float3 Toruqe(float3 a, float3 b, float3 F) {
	float theta = acos(dot(a, b) / abs(a) * abs(b));
	float radius = a.magnitude;

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

float3 SimulateHair(float3 position, float3 tangent, float3 force, float deltaTime, float3 currentSpeed, float3 currentAcceleration)
#endif