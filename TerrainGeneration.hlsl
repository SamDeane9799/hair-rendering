#include "HelperMethods.hlsli"
RWTexture2D<float> heightMap;

[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID)
{
	float x = (DTid.x / 256.0f);
	float y = (DTid.y / 256.0f);
	heightMap[float2(DTid.x, DTid.y)] = noise(float3(x * 7.0f, y * 7.0f, 1)) * 0.5f + 0.5f;
}