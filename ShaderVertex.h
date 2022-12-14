#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct ShaderVertex
{
	DirectX::XMFLOAT3 Position;	    // The position of the vertex
	DirectX::XMFLOAT3 Normal;		// Lighting
	DirectX::XMFLOAT3 Tangent;		// Normal mapping
	DirectX::XMFLOAT2 UV;			// Texture mapping
	float padding;
};