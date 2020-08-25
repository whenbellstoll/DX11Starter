#pragma once
#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> 
class Mesh
{
public:
	Mesh( Vertex * v,  int vIndex, unsigned int * indices, int iIndex, Microsoft::WRL::ComPtr<ID3D11Device> device );
	~Mesh();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();
private:
	// Buffers to hold the model
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
};

