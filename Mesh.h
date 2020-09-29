#pragma once
#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h>
#include "Vertex.h"
#include <fstream>
#include <vector>

class Mesh
{
public:
	Mesh();
	Mesh( Vertex v[],  int vIndex, unsigned int indices[], int iIndex, Microsoft::WRL::ComPtr<ID3D11Device> device );
	Mesh(const char* file, Microsoft::WRL::ComPtr<ID3D11Device> device);
	~Mesh();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();
private:
	int index;
	// Buffers to hold the model
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	void CreateBuffers( Vertex * v, int vIndex, unsigned int* indices, int iIndex, Microsoft::WRL::ComPtr<ID3D11Device> device);
};

