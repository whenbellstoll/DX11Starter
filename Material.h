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
	// Texture Fields
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampleState;
	//  Specular
	float specularValue;
	float specularExpo;
public:	
	// Constructor
	Material(DirectX::XMFLOAT4 color, SimplePixelShader* pS, SimpleVertexShader* vS, float specularV, float specularE, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sr, Microsoft::WRL::ComPtr<ID3D11SamplerState> samS);
	// Getters
	DirectX::XMFLOAT4 GetColorTint();
	SimplePixelShader* GetPixelShader();
	SimpleVertexShader* GetVertexShader();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSRV();
	Microsoft::WRL::ComPtr<ID3D11SamplerState> GetSampleState();
	float GetSpec();
	float GetSpecExpo();
};

