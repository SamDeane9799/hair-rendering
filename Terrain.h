#pragma once
#include "GameEntity.h"
class Terrain : public GameEntity
{
public:
		Terrain(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, float dimension, Microsoft::WRL::ComPtr<ID3D11Device> device);
		inline float GetDimension() { return dimension; }
		inline Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetHeightSRV() { return heightSRV; }

		void CreateTerrain(float dimension, Microsoft::WRL::ComPtr<ID3D11Device> device);

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> heightMap;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> heightUAV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> heightSRV;


	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera);

	void GenerateTerrain(Microsoft::WRL::ComPtr<ID3D11Device> device);
	void CreateBufferResources(float dimension, Microsoft::WRL::ComPtr<ID3D11Device> device);

	float dimension;
};

