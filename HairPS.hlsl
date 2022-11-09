#include "HelperMethods.hlsli"
#include "Lighting.hlsli"

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

struct PS_Output
{
	float4 color	: SV_TARGET0;
	float4 normals	: SV_TARGET1;
	float4 depths	: SV_TARGET2;
	float2 velocity	: SV_TARGET3;
};

cbuffer HairConstants	: register(b2) {
	float metalVal;
	float roughnessVal;
}

#define MAX_LIGHTS 128

// Data that can change per material
cbuffer perMaterial : register(b0)
{
	// Surface color
	float3 colorTint;

	// UV adjustments
	float2 uvScale;
	float2 uvOffset;
};

// Data that only changes once per frame
cbuffer perFrame : register(b1)
{
	// An array of light data
	Light lights[MAX_LIGHTS];

	// The amount of lights THIS FRAME
	int lightCount;

	// Needed for specular (reflection) calculation
	float3 cameraPosition;

	int specIBLTotalMipLevels;
};

SamplerState BasicSampler	: register(s0) { AddressU = Clamp; AddressV = Wrap; };
SamplerState ClampSampler	: register(s1);
//IBL
Texture2D BrdfLookUpMap		: register(t0);
TextureCube IrradianceIBLMap	: register(t1);
TextureCube SpecularIBLMap	: register(t2);
Texture2D NormalMap			: register(t3);

PS_Output main(VertexToPixel input) : SV_TARGET
{
	if (input.uv.x > .7f || input.uv.x > 0.3f)
	{
		discard;
	}
	PS_Output output;

	float sinVal = input.uv.y * (3.14159265f/2.0f);
	float blendVal = sin(sinVal);
	float randVal = abs(perlin(input.uv.x, input.uv.y)) * 1.0f;
	blendVal -= randVal;
	blendVal = clamp(blendVal, 0, 1);
	float4 baseColor = float4(0.38f, 0.35f, 0.27f, 1.0f);
	float4 tipColor = float4(0.98f, 0.94f, 0.74f, 1.0f);
	float4 surfaceColor = lerp(baseColor, tipColor, blendVal);

	// Always re-normalize interpolated direction vectors
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	input.normal = NormalMapping(NormalMap, BasicSampler, input.uv, input.normal, input.tangent);

	float roughness = roughnessVal;
	float metal = metalVal;

	// Gamma correct the texture back to linear space and apply the color tint
	surfaceColor.rgb = pow(surfaceColor.rgb, 2.2);

	// Specular color - Assuming albedo texture is actually holding specular color if metal == 1
	// Note the use of lerp here - metal is generally 0 or 1, but might be in between
	// because of linear texture sampling, so we want lerp the specular color to match
	float3 specColor = lerp(F0_NON_METAL.rrr, surfaceColor.rgb, metal);

	// Total color for this pixel
	float3 totalColor = float3(0, 0, 0);

	// Loop through all lights this frame
	for (int i = 0; i < lightCount; i++)
	{
		// Which kind of light?
		switch (lights[i].Type)
		{
		case LIGHT_TYPE_DIRECTIONAL:
			totalColor += DirLightPBR(lights[i], input.normal, input.worldPos, cameraPosition, roughness, metal, surfaceColor.rgb, specColor);
			break;

		case LIGHT_TYPE_POINT:
			totalColor += PointLightPBR(lights[i], input.normal, input.worldPos, cameraPosition, roughness, metal, surfaceColor.rgb, specColor);
			break;

		case LIGHT_TYPE_SPOT:
			totalColor += SpotLightPBR(lights[i], input.normal, input.worldPos, cameraPosition, roughness, metal, surfaceColor.rgb, specColor);
			break;
		}
	}

	// Calculate requisite reflection vectors

	float3 viewToCam = normalize(cameraPosition - input.worldPos);

	float3 viewRefl = normalize(reflect(-viewToCam, input.normal));

	float NdotV = saturate(dot(input.normal, viewToCam));


	// Indirect lighting

	float3 indirectDiffuse = IndirectDiffuse(IrradianceIBLMap, BasicSampler, input.normal);

	float3 indirectSpecular = IndirectSpecular(

		SpecularIBLMap, specIBLTotalMipLevels,

		BrdfLookUpMap, ClampSampler, // MUST use the clamp sampler here!

		viewRefl, NdotV,

		roughness, specColor);


	// Balance indirect diff/spec

	float3 balancedDiff = DiffuseEnergyConserve(indirectDiffuse, indirectSpecular, metal);

	float3 fullIndirect =  balancedDiff * surfaceColor.rgb;

	// Add the indirect to the direct

	totalColor += fullIndirect;


	// Gamma correction
	output.color = float4(pow(totalColor, 1.0f / 2.2f), 1);
	output.velocity = float4(0,0,0,0);
	output.normals = float4(input.normal, 1);
	output.depths = float4(input.screenPosition.z, 0, 0, 1);
	return output;
}