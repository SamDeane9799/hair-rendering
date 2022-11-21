struct HairStrand
{
	float3 Position;	    // The position of the vertex
	float3 Normal;		// Lighting
	float3 Tangent;		//Lighting
	float2 UV;			// Texture mapping
	float3 Acceleration;// Physics Simulation
	float3 Speed;		//Physics Simulation
	float3 padding;
};