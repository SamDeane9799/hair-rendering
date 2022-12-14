#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include "Vertex.h"


class Mesh
{
public:
	Mesh(Vertex* vertArray, int numVerts, unsigned int* indexArray, int numIndices, Microsoft::WRL::ComPtr<ID3D11Device> device);
	Mesh(Vertex* vertArray, int numVerts, unsigned int* indexArray, int numIndices, Microsoft::WRL::ComPtr<ID3D11Device> device, bool hasFur);
	Mesh(const char* objFile, Microsoft::WRL::ComPtr<ID3D11Device> device, bool hasFur);
	~Mesh(void);

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer() { return vb; }
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer() { return ib; }
	bool GetHasFur() { return hasFur; }
	int GetIndexCount() { return numIndices; }

	void SetBuffersAndDraw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

	void SimulateHair(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Device> device, float deltaTime, DirectX::XMFLOAT3 force);
	void SetBuffersAndCreateHair(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	void SetBuffersAndDrawHair(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalMap);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vb;
	Microsoft::WRL::ComPtr<ID3D11Buffer> sb;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderVertexSRV;
	Microsoft::WRL::ComPtr<ID3D11Buffer> initialHairBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> newHairBuffer;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> hairUAV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> hairSRV;
	Microsoft::WRL::ComPtr<ID3D11Buffer> ib;
	Microsoft::WRL::ComPtr<ID3D11Buffer> hairIB;
	int numIndices;
	int numOfVerts;
	bool hasFur;

	void CreateBuffers(Vertex* vertArray, int numVerts, unsigned int* indexArray, int numIndices, Microsoft::WRL::ComPtr<ID3D11Device> device);
	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
	void CreateHairBuffers(Vertex* vertArray, int numVerts, Microsoft::WRL::ComPtr<ID3D11Device> device);
};

