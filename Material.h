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
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalMap;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughMap;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalMap;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampleState;
	//  Specular
	float specularValue;
	float specularExpo;
	bool normalMapBool;
	bool PBRBool;
public:	
	// Constructor
	Material(DirectX::XMFLOAT4 color, SimplePixelShader* pS, SimpleVertexShader* vS, float specularV, float specularE, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sr, Microsoft::WRL::ComPtr<ID3D11SamplerState> samS);
	Material(DirectX::XMFLOAT4 color, SimplePixelShader* pS, SimpleVertexShader* vS, float specularV, float specularE, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sr, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> nm, Microsoft::WRL::ComPtr<ID3D11SamplerState> samS);
	Material(DirectX::XMFLOAT4 color, SimplePixelShader* pS, SimpleVertexShader* vS, float specularV, float specularE, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sr, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> nm, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> rm, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mm, Microsoft::WRL::ComPtr<ID3D11SamplerState> samS);
	// Getters
	bool IsNormal();
	bool IsPBR();
	DirectX::XMFLOAT4 GetColorTint();
	SimplePixelShader* GetPixelShader();
	SimpleVertexShader* GetVertexShader();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSRV();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetNormal();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetRough();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMetal();
	Microsoft::WRL::ComPtr<ID3D11SamplerState> GetSampleState();
	float GetSpec();
	float GetSpecExpo();
};

