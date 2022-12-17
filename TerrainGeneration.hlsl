#include "HelperMethods.hlsli"
RWTexture2D<float> heightMap;

[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID)
{
	heightMap[float2(DTid.x, DTid.y)] = perlin((DTid.x / 256.0f), (DTid.y / 256.0f)) * 0.5f + 0.5f;
}