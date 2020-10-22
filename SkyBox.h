#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "Camera.h"
#include <DirectXMath.h>
#include <wrl/client.h>
class SkyBox
{
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvSky;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> stencilState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterState;
	Mesh* cube;
	SimplePixelShader* ps;
	SimpleVertexShader* vs;
public:
	SkyBox(Microsoft::WRL::ComPtr<ID3D11SamplerState> sO, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, Microsoft::WRL::ComPtr<ID3D11Device> device, Mesh * c, SimplePixelShader * sps, SimpleVertexShader * svs);
	~SkyBox();

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* cam);
};

