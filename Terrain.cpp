#include "Terrain.h"
#include "SimpleShader.h"
#include "Assets.h"

Terrain::Terrain(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, Microsoft::WRL::ComPtr<ID3D11Device> device, float dimension, float frequency)
	:GameEntity(mesh, material)
{
	CreateTerrain(dimension, frequency, device);

	GetTransform()->MoveAbsolute(0, -5, 0);
	GetTransform()->SetScale(10, 1, 10);
}

void Terrain::CreateTerrain(float dimension, float frequency, Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	CreateBufferResources(dimension, device);
	this->dimension = dimension;
	this->frequency = frequency;

	GenerateTerrain(device);
}

void Terrain::GenerateTerrain(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	if (heightSRV != NULL) {
		heightSRV.Reset();
	}
	SimpleComputeShader* terrainGenCS = Assets::GetInstance().GetComputeShader("TerrainGeneration").get();

	terrainGenCS->SetShader();
	terrainGenCS->SetUnorderedAccessView("heightMap", heightUAV);
	terrainGenCS->SetFloat("frequency", frequency);
	terrainGenCS->CopyAllBufferData();
	terrainGenCS->DispatchByThreads(dimension, dimension, 1);
	terrainGenCS->SetUnorderedAccessView("heightMap", 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC heightSRVDesc = {};
	heightSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	heightSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	heightSRVDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(heightMap.Get(), &heightSRVDesc, heightSRV.GetAddressOf());
}

void Terrain::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera)
{
	GetMaterial()->GetVertexShader()->SetShader();
	GetMaterial()->GetVertexShader()->SetShaderResourceView("HeightMap", heightSRV);
	GameEntity::Draw(context, camera);
}

void Terrain::CreateBufferResources(float dimension, Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	if (heightMap != NULL) {
		heightMap.Reset();
		heightUAV.Reset();
	}

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.MipLevels = texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.Format = DXGI_FORMAT_R32_FLOAT;
	texDesc.Width = dimension;
	texDesc.Height = dimension;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	texDesc.MiscFlags = 0;

	device->CreateTexture2D(&texDesc, NULL, heightMap.GetAddressOf());

	D3D11_UNORDERED_ACCESS_VIEW_DESC heightMapUAVDesc = {};
	heightMapUAVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	heightMapUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	device->CreateUnorderedAccessView(heightMap.Get(), &heightMapUAVDesc, heightUAV.GetAddressOf());
}
