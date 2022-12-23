
struct PS_Output
{
	float4 color	: SV_TARGET0;
	float4 normals	: SV_TARGET1;
	float4 depths	: SV_TARGET2;
	float2 velocity	: SV_TARGET3;
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

PS_Output main(VertexToPixel input)
{
	PS_Output output;
	output.color = float4(input.elevation, input.elevation, input.elevation, 1.0f);
	return output;
}