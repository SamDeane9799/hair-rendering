#pragma once

#include "DXCore.h"
#include "Mesh.h"
#include "GameEntity.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "SpriteFont.h"
#include "SpriteBatch.h"
#include "Lights.h"
#include "Sky.h"
#include "Renderer.h"
#include "Emitter.h"
#include "Terrain.h"

#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <vector>

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:

	// Our scene
	std::vector<std::shared_ptr<GameEntity>> entities;
	std::vector<std::shared_ptr<Material>> materials;
	std::vector<std::shared_ptr<Emitter>> emitter;
	std::shared_ptr<Camera> camera;

	int entityDirection = 1;

	// Lights
	std::vector<Light> lights;
	int lightCount;

	std::unique_ptr<Renderer> DXRenderer;

	// Texture related resources
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> clampSamplerOptions;

	// Skybox
	std::shared_ptr<Sky> sky;
	std::shared_ptr<Terrain> terrain;

	// General helpers for setup and drawing
	void GenerateLights();

	// Initialization helper method
	void LoadAssetsAndCreateEntities();
};

