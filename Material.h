#pragma once
#include <DirectXMath.h>
#include <wrl/client.h>
#include "DXCore.h"
class Material
{
	// Fields
	DirectX::XMFLOAT4 colorTint;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	// Constructor
	Material(DirectX::XMFLOAT4 color, Microsoft::WRL::ComPtr<ID3D11PixelShader> pS, Microsoft::WRL::ComPtr<ID3D11VertexShader> vS);
	// Getters
	DirectX::XMFLOAT4 GetColorTint();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetVertexShader();
};

