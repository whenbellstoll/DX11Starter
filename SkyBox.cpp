#include "SkyBox.h"

SkyBox::SkyBox(Microsoft::WRL::ComPtr<ID3D11SamplerState> sO, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, Microsoft::WRL::ComPtr<ID3D11Device> device, Mesh* c, SimplePixelShader* sps, SimpleVertexShader* svs)
{
	samplerOptions = sO;
	srvSky = srv;
	cube = c;
	ps = sps;
	vs = svs;

	// Rasterizer initialization
	D3D11_RASTERIZER_DESC ras = {};
	ras.FillMode = D3D11_FILL_SOLID;
	ras.CullMode = D3D11_CULL_FRONT;

	device->CreateRasterizerState(&ras, rasterState.GetAddressOf());

	// Depth Stencil State initialization
	D3D11_DEPTH_STENCIL_DESC dep = {};
	dep.DepthEnable = true;
	dep.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	device->CreateDepthStencilState(&dep, stencilState.GetAddressOf());

}

SkyBox::~SkyBox()
{
	// initalize all our pointers in Game.cpp, should delete them there.
}

void SkyBox::Draw()
{

}
