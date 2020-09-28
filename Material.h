#pragma once
#include <DirectXMath.h>
#include <wrl/client.h>
#include "DXCore.h"
#include "SimpleShader.h"

class Material
{
	// Fields
	DirectX::XMFLOAT4 colorTint;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
public:	
	// Constructor
	Material(DirectX::XMFLOAT4 color, SimplePixelShader* pS, SimpleVertexShader* vS);
	// Getters
	DirectX::XMFLOAT4 GetColorTint();
	SimplePixelShader* GetPixelShader();
	SimpleVertexShader* GetVertexShader();
};

