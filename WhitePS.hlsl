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

float4 main(VertexToPixel input) : SV_TARGET
{
	float sinVal = input.uv.y * (3.14159265f/2.0f);
	float blendVal = sin(sinVal);
	float4 baseColor = float4(0.38f, 0.35f, 0.27f, 1.0f);
	float4 tipColor = float4(0.98f, 0.94f, 0.74f, 1.0f);
	float4 colorLerpVal = lerp(baseColor, tipColor, blendVal);

	return colorLerpVal;
}