// Include guard
#ifndef _HAIRPHYSICSHELPER_HLSL
#define _HAIRPHYSICSHELPER_HLSL
#define HAIR_WEIGHT 0.1f
#define MAX_ACCELERATION float3(0.5f, 0.5f, 0.5f)
#define MIN_ACCELERATION float3(-0.5f, -0.5f, -0.5f)
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
float3 AccelerationCalc(float3 force, float mass) {
	return (force * mass);
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

HairStrand SimulateHair(HairStrand strand, float3 force, float deltaTime, float2x3 constraints)
{
	//Find how close our position is to a constraint
	//If we've reached it or surpassed it we want to eliminate force in that direction
	//And stop any movement/acceleration in that direction
	float3 minConstraint = float3(constraints._m00, constraints._m01, constraints._m02);
	float3 maxConstraint = float3(constraints._m10, constraints._m11, constraints._m12);
	float3 minDiff = strand.Position - minConstraint;
	float3 minConstraintDiff = clamp(ceil(minDiff), float3(0, 0, 0), float3(1, 1, 1));
	float3 maxDiff = maxConstraint - strand.Position;
	float3 maxConstraintDiff = clamp(ceil(maxDiff), float3(0, 0, 0), float3(1, 1, 1));

	float3 minAccel = minConstraintDiff * MIN_ACCELERATION;
	float3 maxAccel = maxConstraintDiff * MAX_ACCELERATION;

	float3 forceDirection = (strand.OriginalPosition - strand.Position);
	if (length(forceDirection) < 0.01f && length(force) == 0) {
		forceDirection = float3(0, 0, 0);
		strand.Acceleration = float3(0, 0, 0);
		strand.Speed = float3(0, 0, 0);
	}
	else {
		float3 acceleration = AccelerationCalc(forceDirection, HAIR_WEIGHT);
		float3 accelerationFromForce = AccelerationCalc(force, HAIR_WEIGHT);
		strand.Acceleration = clamp(strand.Acceleration + (acceleration + accelerationFromForce), minAccel, maxAccel);
		float3 speed = SpeedCalc(strand.Acceleration, strand.Speed, deltaTime);
		strand.Speed = clamp(speed, minAccel, maxAccel);
	}

	float3 offset = (strand.Speed * deltaTime);

	strand.Position = clamp((strand.Position + offset), minConstraint, maxConstraint);

	return strand;
}
#endif