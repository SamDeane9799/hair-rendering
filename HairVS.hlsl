#include "HairGenerics.hlsli"

cbuffer externalData : register(b0)
{
	matrix world;
	matrix worldInverseTranspose;
	matrix view;
	matrix projection;
};
// Out of the vertex shader (and eventually input to the PS)
struct VertexToPixel
{
	float4 screenPosition	: SV_POSITION;
	float2 uv				: TEXCOORD;
	float3 normal			: NORMAL;
	float3 tangent			: TANGENT;
	float3 worldPos			: POSITION; // The world position of this vertex
	float4 prevScreenPos	: SCREEN_POS0;// The world position of this vertex last frame
	float4 currentScreenPos	: SCREEN_POS1;
};

StructuredBuffer<HairStrand> HairData	:	register(t0);

VertexToPixel main(uint id : SV_VertexID)
{
	// Set up output
	VertexToPixel output;
	HairStrand input = HairData.Load(id);
	//input.Position = input.Position - float3(0, -2.5f, -5.0f);
	// Calculate output position
	matrix worldViewProj = mul(projection, mul(view, world));
	output.screenPosition = mul(worldViewProj, float4(input.Position, 1.0f));
	output.currentScreenPos = output.screenPosition;

	// Calculate the world position of this vertex (to be used
	// in the pixel shader when we do point/spot lights)
	output.worldPos = mul(world, float4(input.Position, 1.0f)).xyz;

	// Make sure the other vectors are in WORLD space, not "local" space
	output.normal = normalize(mul((float3x3)worldInverseTranspose, input.Normal));
	output.tangent = normalize(mul((float3x3)world, input.Tangent)); // Tangent doesn't need inverse transpose!

	// Pass the UV through
	output.uv = input.UV;

	return output;
}