
// Constant Buffer for external (C++) data
cbuffer externalData : register(b0)
{
	matrix world;
	matrix prevWorld;
	matrix worldInverseTranspose;
	matrix view;
	matrix projection;
	matrix prevView;
	matrix prevProjection;
};

// Struct representing a single vertex worth of data
struct VertexShaderInput
{
	float3 position		: POSITION;
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
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
	float elevation			: PSIZE;
};

Texture2D<float> HeightMap: register(t0);
SamplerState BasicSampler	: register(s0);

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixel main(VertexShaderInput input)
{
	// Set up output
	VertexToPixel output;

	float height = HeightMap.SampleLevel(BasicSampler, input.uv, 0) * 20.0f;
	output.elevation = height / 5.0f;
	float3 newPosition = input.position + float3(0, height, 0);

	// Calculate output position
	matrix worldViewProj = mul(projection, mul(view, world));
	output.screenPosition = mul(worldViewProj, float4(newPosition, 1.0f));
	output.currentScreenPos = output.screenPosition;

	matrix prevWorldViewProj = mul(prevProjection, mul(prevView, prevWorld));
	output.prevScreenPos = mul(prevWorldViewProj, float4(newPosition, 1.0f));

	// Make sure the other vectors are in WORLD space, not "local" space
	output.normal = normalize(mul((float3x3)worldInverseTranspose, input.normal));
	output.tangent = normalize(mul((float3x3)world, input.tangent)); // Tangent doesn't need inverse transpose!

	// Calculate the world position of this vertex (to be used
	// in the pixel shader when we do point/spot lights)
	output.worldPos = mul(world, float4(newPosition, 1.0f)).xyz;

	// Pass the UV through
	output.uv = input.uv;

	return output;
}