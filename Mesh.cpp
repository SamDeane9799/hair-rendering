#include "Mesh.h"
#include "Assets.h"
#include "HairStrand.h"
#include "ShaderVertex.h"
#include <memory>
#include <DirectXMath.h>
#include <vector>
#include <fstream>

using namespace DirectX;

Mesh::Mesh(Vertex* vertArray, int numVerts, unsigned int* indexArray, int numIndices, Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	numOfVerts = numVerts;
	CreateBuffers(vertArray, numVerts, indexArray, numIndices, device);
	hasFur = false;
}

Mesh::Mesh(Vertex* vertArray, int numVerts, unsigned int* indexArray, int numIndices, Microsoft::WRL::ComPtr<ID3D11Device> device, bool hasFur) : Mesh(vertArray, numVerts, indexArray, numIndices, device)
{
	if (hasFur)
		CreateHairBuffers(vertArray, numVerts, device);
	this->hasFur = hasFur;
}

Mesh::Mesh(const char* objFile, Microsoft::WRL::ComPtr<ID3D11Device> device, bool hasFur)
{
	// File input object
	std::ifstream obj(objFile);

	// Check for successful open
	if (!obj.is_open())
		return;

	// Variables used while reading the file
	std::vector<XMFLOAT3> positions;     // Positions from the file
	std::vector<XMFLOAT3> normals;       // Normals from the file
	std::vector<XMFLOAT2> uvs;           // UVs from the file
	std::vector<Vertex> verts;           // Verts we're assembling
	std::vector<UINT> indices;           // Indices of these verts
	unsigned int vertCounter = 0;        // Count of vertices/indices
	char chars[100];                     // String for line reading

	// Still have data left?
	while (obj.good())
	{
		// Get the line (100 characters should be more than enough)
		obj.getline(chars, 100);

		// Check the type of line
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			XMFLOAT3 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z);

			// Add to the list of normals
			normals.push_back(norm);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Read the 2 numbers directly into an XMFLOAT2
			XMFLOAT2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y);

			// Add to the list of uv's
			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			XMFLOAT3 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z);

			// Add to the positions
			positions.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			// Read the face indices into an array
			// NOTE: This assumes the given obj file contains
			//  vertex positions, uv coordinates AND normals.
			//  If the model is missing any of these, this 
			//  code will not handle the file correctly!
			unsigned int i[12];
			int facesRead = sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8],
				&i[9], &i[10], &i[11]);

			// - Create the verts by looking up
			//    corresponding data from vectors
			// - OBJ File indices are 1-based, so
			//    they need to be adusted
			Vertex v1;
			v1.Position = positions[i[0] - 1];
			v1.UV = uvs[i[1] - 1];
			v1.Normal = normals[i[2] - 1];

			Vertex v2;
			v2.Position = positions[i[3] - 1];
			v2.UV = uvs[i[4] - 1];
			v2.Normal = normals[i[5] - 1];

			Vertex v3;
			v3.Position = positions[i[6] - 1];
			v3.UV = uvs[i[7] - 1];
			v3.Normal = normals[i[8] - 1];

			// The model is most likely in a right-handed space,
			// especially if it came from Maya.  We want to convert
			// to a left-handed space for DirectX.  This means we 
			// need to:
			//  - Invert the Z position
			//  - Invert the normal's Z
			//  - Flip the winding order
			// We also need to flip the UV coordinate since DirectX
			// defines (0,0) as the top left of the texture, and many
			// 3D modeling packages use the bottom left as (0,0)

			// Flip the UV's since they're probably "upside down"
			v1.UV.y = 1.0f - v1.UV.y;
			v2.UV.y = 1.0f - v2.UV.y;
			v3.UV.y = 1.0f - v3.UV.y;

			// Flip Z (LH vs. RH)
			v1.Position.z *= -1.0f;
			v2.Position.z *= -1.0f;
			v3.Position.z *= -1.0f;

			// Flip normal Z
			v1.Normal.z *= -1.0f;
			v2.Normal.z *= -1.0f;
			v3.Normal.z *= -1.0f;

			// Add the verts to the vector (flipping the winding order)
			verts.push_back(v1);
			verts.push_back(v3);
			verts.push_back(v2);

			// Add three more indices
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;

			// Was there a 4th face?
			if (facesRead == 12)
			{
				// Make the last vertex
				Vertex v4;
				v4.Position = positions[i[9] - 1];
				v4.UV = uvs[i[10] - 1];
				v4.Normal = normals[i[11] - 1];

				// Flip the UV, Z pos and normal
				v4.UV.y = 1.0f - v4.UV.y;
				v4.Position.z *= -1.0f;
				v4.Normal.z *= -1.0f;

				// Add a whole triangle (flipping the winding order)
				verts.push_back(v1);
				verts.push_back(v4);
				verts.push_back(v3);

				// Add three more indices
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
			}
		}
	}

	// Close the file and create the actual buffers
	obj.close();


	// - At this point, "verts" is a vector of Vertex structs, and can be used
	//    directly to create a vertex buffer:  &verts[0] is the address of the first vert
	//
	// - The vector "indices" is similar. It's a vector of unsigned ints and
	//    can be used directly for the index buffer: &indices[0] is the address of the first int
	//
	// - "vertCounter" is BOTH the number of vertices and the number of indices
	// - Yes, the indices are a bit redundant here (one per vertex).  Could you skip using
	//    an index buffer in this case?  Sure!  Though, if your mesh class assumes you have
	//    one, you'll need to write some extra code to handle cases when you don't.

	numOfVerts = vertCounter;

	CreateBuffers(&verts[0], vertCounter, &indices[0], vertCounter, device);

	if (hasFur)
		CreateHairBuffers(&verts[0], vertCounter, device);
	this->hasFur = hasFur;
}


Mesh::~Mesh(void)
{

}


void Mesh::SetBuffersAndDrawHair(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	// Set buffers in the input assembler
	UINT stride = 0;
	UINT offset = 0;
	ID3D11Buffer* nullBuffer = 0;
	context->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
	context->IASetIndexBuffer(hairIB.Get(), DXGI_FORMAT_R32_UINT, 0);

	std::shared_ptr<SimpleVertexShader> vs = Assets::GetInstance().GetVertexShader("HairVS");
	vs->SetShader();
	vs->SetShaderResourceView("HairData", hairSRV);
	vs->CopyAllBufferData();

	std::shared_ptr<SimplePixelShader> ps = Assets::GetInstance().GetPixelShader("WhitePS");
	ps->SetShader();

	// Draw this mesh's hair
	//TODO: Move this into renderer and move the rasterizer desc there as well
	//context->RSSetState(hairRast.Get());
	context->DrawIndexed(this->numOfVerts * 3, 0, 0);
}

void Mesh::CreateBuffers(Vertex* vertArray, int numVerts, unsigned int* indexArray, int numIndices, Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	// Always calculate the tangents before copying to buffer
	CalculateTangents(vertArray, numVerts, indexArray, numIndices);


	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * numVerts; // Number of vertices
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertArray;
	device->CreateBuffer(&vbd, &initialVertexData, vb.GetAddressOf());

	// Create the index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * numIndices; // Number of indices
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indexArray;
	device->CreateBuffer(&ibd, &initialIndexData, ib.GetAddressOf());

	// Save the indices
	this->numIndices = numIndices;
}


// Calculates the tangents of the vertices in a mesh
// Code originally adapted from: http://www.terathon.com/code/tangent.html
// Updated version now found here: http://foundationsofgameenginedev.com/FGED2-sample.pdf
//  - See listing 7.4 in section 7.5 (page 9 of the PDF)
void Mesh::CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices)
{
	// Reset tangents
	for (int i = 0; i < numVerts; i++)
	{
		verts[i].Tangent = XMFLOAT3(0, 0, 0);
	}

	// Calculate tangents one whole triangle at a time
	for (int i = 0; i < numIndices;)
	{
		// Grab indices and vertices of first triangle
		unsigned int i1 = indices[i++];
		unsigned int i2 = indices[i++];
		unsigned int i3 = indices[i++];
		Vertex* v1 = &verts[i1];
		Vertex* v2 = &verts[i2];
		Vertex* v3 = &verts[i3];

		// Calculate vectors relative to triangle positions
		float x1 = v2->Position.x - v1->Position.x;
		float y1 = v2->Position.y - v1->Position.y;
		float z1 = v2->Position.z - v1->Position.z;

		float x2 = v3->Position.x - v1->Position.x;
		float y2 = v3->Position.y - v1->Position.y;
		float z2 = v3->Position.z - v1->Position.z;

		// Do the same for vectors relative to triangle uv's
		float s1 = v2->UV.x - v1->UV.x;
		float t1 = v2->UV.y - v1->UV.y;

		float s2 = v3->UV.x - v1->UV.x;
		float t2 = v3->UV.y - v1->UV.y;

		// Create vectors for tangent calculation
		float r = 1.0f / (s1 * t2 - s2 * t1);
		
		float tx = (t2 * x1 - t1 * x2) * r;
		float ty = (t2 * y1 - t1 * y2) * r;
		float tz = (t2 * z1 - t1 * z2) * r;

		// Adjust tangents of each vert of the triangle
		v1->Tangent.x += tx; 
		v1->Tangent.y += ty; 
		v1->Tangent.z += tz;

		v2->Tangent.x += tx; 
		v2->Tangent.y += ty; 
		v2->Tangent.z += tz;

		v3->Tangent.x += tx; 
		v3->Tangent.y += ty; 
		v3->Tangent.z += tz;
	}

	// Ensure all of the tangents are orthogonal to the normals
	for (int i = 0; i < numVerts; i++)
	{
		// Grab the two vectors
		XMVECTOR normal = XMLoadFloat3(&verts[i].Normal);
		XMVECTOR tangent = XMLoadFloat3(&verts[i].Tangent);

		// Use Gram-Schmidt orthogonalize
		tangent = XMVector3Normalize(
			tangent - normal * XMVector3Dot(normal, tangent));
		
		// Store the tangent
		XMStoreFloat3(&verts[i].Tangent, tangent);
	}
}




void Mesh::SetBuffersAndDraw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	// Set buffers in the input assembler
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, vb.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(ib.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Draw this mesh
	context->DrawIndexed(this->numIndices, 0, 0);
}

void Mesh::SetBuffersAndCreateHair(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	std::shared_ptr<SimpleComputeShader> hairCS = Assets::GetInstance().GetComputeShader("CreateHair");

	hairCS->SetShader();
	hairCS->SetShaderResourceView("vertexData", shaderVertexSRV);
	hairCS->SetUnorderedAccessView("hairData", hairUAV, 0);
	hairCS->SetFloat("length", 0.5f);
	hairCS->SetFloat("width", .01f);
	hairCS->CopyAllBufferData();
	hairCS->DispatchByThreads(numOfVerts * 3, 1, 1);

	D3D11_BUFFER_DESC newHairBufferDesc;
	newHairBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	newHairBufferDesc.ByteWidth = (sizeof(HairStrand)) * numOfVerts; // Number of vertices
	newHairBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	newHairBufferDesc.CPUAccessFlags = 0;
	newHairBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	newHairBufferDesc.StructureByteStride = sizeof(HairStrand);
	device->CreateBuffer(&newHairBufferDesc, 0, newHairBuffer.GetAddressOf());
	context->CopyResource(newHairBuffer.Get(), initialHairBuffer.Get());

	D3D11_SHADER_RESOURCE_VIEW_DESC hairSRVDesc = {};
	hairSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	hairSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	hairSRVDesc.Buffer.NumElements = numOfVerts;
	hairSRVDesc.Buffer.FirstElement = 0;
	device->CreateShaderResourceView(newHairBuffer.Get(), &hairSRVDesc, hairSRV.GetAddressOf());
}

void Mesh::CreateHairBuffers(Vertex* vertArray, int numVerts, Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	ShaderVertex* vertexInfo = new ShaderVertex[numVerts];
	for (int i = 0; i < numVerts; i++)
	{
		vertexInfo[i].Position = vertArray[i].Position;
		vertexInfo[i].Normal = vertArray[i].Normal;
		vertexInfo[i].Tangent = vertArray[i].Tangent;
		vertexInfo[i].UV = vertArray[i].UV;
		vertexInfo[i].padding = 0;
	}

	// Create the vertex buffer
	D3D11_BUFFER_DESC sbd;
	sbd.Usage = D3D11_USAGE_IMMUTABLE;
	sbd.ByteWidth = (sizeof(ShaderVertex)) * numVerts; // Number of vertices
	sbd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sbd.CPUAccessFlags = 0;
	sbd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	sbd.StructureByteStride = sizeof(ShaderVertex);
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertexInfo;
	device->CreateBuffer(&sbd, &initialVertexData, sb.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderVertexDesc = {};
	shaderVertexDesc.Format = DXGI_FORMAT_UNKNOWN;
	shaderVertexDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	shaderVertexDesc.Buffer.NumElements = numVerts;
	shaderVertexDesc.Buffer.FirstElement = 0;
	shaderVertexDesc.Buffer.ElementWidth = sizeof(ShaderVertex);
	device->CreateShaderResourceView(sb.Get(), &shaderVertexDesc, shaderVertexSRV.GetAddressOf());

	//Create buffer for information on hair
	D3D11_BUFFER_DESC hbd;
	hbd.Usage = D3D11_USAGE_DEFAULT;
	hbd.ByteWidth = sizeof(HairStrand) * numVerts; 
	hbd.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	hbd.CPUAccessFlags = 0;
	hbd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	hbd.StructureByteStride = sizeof(HairStrand);
	device->CreateBuffer(&hbd, 0, initialHairBuffer.GetAddressOf());

	D3D11_UNORDERED_ACCESS_VIEW_DESC hairDesc = {};
	hairDesc.Format = DXGI_FORMAT_UNKNOWN;
	hairDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	hairDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
	hairDesc.Buffer.FirstElement = 0;
	hairDesc.Buffer.NumElements = numVerts;
	device->CreateUnorderedAccessView(initialHairBuffer.Get(), &hairDesc, hairUAV.GetAddressOf());

	//Create hair index buffer
	int numIndices = numOfVerts * 3;
	unsigned int* indicies = new unsigned int[numIndices];
	for (int i = 0; i < numIndices; i ++)
	{
		indicies[i] = i;
	}

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indicies;

	D3D11_BUFFER_DESC ibDesc = {};
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.ByteWidth = sizeof(unsigned int) * numIndices;
	device->CreateBuffer(&ibDesc, &indexData, hairIB.GetAddressOf());

	D3D11_RASTERIZER_DESC hairRastDesc;
	hairRastDesc.CullMode = D3D11_CULL_NONE;
	hairRastDesc.FillMode = D3D11_FILL_SOLID;
	hairRastDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&hairRastDesc, hairRast.GetAddressOf());

	delete[] indicies;
	delete[] vertexInfo;
}
