#include "HelperMethods.hlsli"
cbuffer HAIR_CONSTANT_BUFFER	: register(b0) {
	float frequency;
}
RWTexture2D<float> heightMap;

[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID)
{
	float x = (DTid.x / 256.0f);
	float y = (DTid.y / 256.0f);
	/*float elevation = (1 * (noise(float3(x * 1.0f, y * 1.0f, 1)) * 0.5f + 0.5f)) +
		(0.5f * (noise(float3(x * 3.0f, y * 3.0f, 1)) * 0.5f + 0.5f)) + 
		(0.25f * (noise(float3(x * 5.0f, y * 5.0f, 1)) * 0.5f + 0.5f));
	heightMap[float2(DTid.x, DTid.y)] = elevation / (1.0f + 0.5f + 0.25f);*/
	heightMap[float2(DTid.x, DTid.y)] = noise(float3(x * frequency, y * frequency, 1)) * 0.5f + 0.5f;
}