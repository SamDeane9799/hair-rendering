#pragma once
#include "GameEntity.h"
class Terrain : GameEntity
{
public:
		Terrain(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, float dimension, Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		inline float GetDimension() { return dimension; }
		inline Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetHeightSRV() { return heightSRV; }
		void GenerateTerrain(Microsoft::WRL::ComPtr<ID3D11Device> device);

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> heightMap;
	Microsoft::WRL::ComPtr<ID3D11Buffer> heightBuffer;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> heightUAV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> heightSRV;

	float dimension;
};

