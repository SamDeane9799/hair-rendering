
#pragma once
#include <d3d11.h>
#include "Mesh.h"
#include "Material.h"
#include "GameEntity.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "SpriteFont.h"
#include "SpriteBatch.h"
#include "Lights.h"
#include "Emitter.h"
#include "Sky.h"
#include "Terrain.h"
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <memory>

enum RenderTargetType
{ 
	ALBEDO,
	NORMALS,
	DEPTHS,
	VELOCITY,
	NEIGHBORHOOD_MAX,
	RENDER_TARGETS_COUNT
};

class Renderer
{
public:
	Renderer(Microsoft::WRL::ComPtr<ID3D11Device> Device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context, Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain, Microsoft::WRL::ComPtr<ID3D11RenderTargetView> BackBufferRTV,
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthBufferDSV, unsigned int WindowWidth, unsigned int WindowHeight, std::shared_ptr<Sky> SkyPTR, std::shared_ptr<Terrain> terrainPTR, std::vector<std::shared_ptr<GameEntity>>& Entities, std::vector<std::shared_ptr<Emitter>>& Emitters,
		std::vector<Light>& Lights, HWND hWnd);
	~Renderer();
	void PreResize();

	void PostResize(unsigned int windowWidth, unsigned int windowHeight,
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBufferRTV, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthBufferDSV);

	void Render(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<Material>> materials, float deltaTime);
private:

	void DrawPointLights(std::shared_ptr<Camera> camera);

	void DrawUI(std::vector<std::shared_ptr<Material>> materials, float deltaTime);

	void CreatePostProcessResources();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBufferRTV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthBufferDSV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> particleDSS;
	Microsoft::WRL::ComPtr<ID3D11BlendState> particleBS;
	std::shared_ptr<DirectX::SpriteFont> arial;
	std::shared_ptr<DirectX::SpriteBatch> spriteBatch;
	unsigned int windowWidth;
	unsigned int windowHeight;
	int motionBlurNeighborhoodSamples;
	int motionBlurMax;
	std::shared_ptr<Sky> sky;
	std::shared_ptr<Terrain> terrain;
	std::vector<std::shared_ptr<GameEntity>>& entities;
	std::vector<std::shared_ptr<Emitter>>& emitters;
	std::vector<Light>& lights;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>* renderTargetsRTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* renderTargetsSRV;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> ppSampler;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> hairRast;

	DirectX::XMFLOAT4X4 prevView;
	DirectX::XMFLOAT4X4 prevProj;

	const int terrainGenDimensions[5] = {64, 128, 256, 512, 1024};
	int terrainDimensions;

	const float frequencyOptions[3] = { 1.0f, 3.0f, 7.0f };
	float frequency;
};

