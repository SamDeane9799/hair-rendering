#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct HairStrand
{
	DirectX::XMFLOAT3 Position;	    // The position of the vertex
	DirectX::XMFLOAT3 Normal;		// Lighting
	DirectX::XMFLOAT3 Tangent;	// Lighting
	DirectX::XMFLOAT3 Acceleration;	// Simulating physics
	DirectX::XMFLOAT3 Speed;	// Simulating physics
	DirectX::XMFLOAT2 UV;			// Texture mapping
	DirectX::XMFLOAT3 OriginalPosition;
};