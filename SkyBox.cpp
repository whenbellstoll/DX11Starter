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

void SkyBox::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera * cam )
{
	context->RSSetState(rasterState.Get());
	context->OMSetDepthStencilState(stencilState.Get(), 0);

	vs->SetShader();
	ps->SetShader();
	
	
	vs->SetMatrix4x4("view", cam->GetViewMatrix() );
	vs->SetMatrix4x4("proj", cam->GetProjMatrix());
	vs->CopyAllBufferData();

	// Ensure the pixel shader has the texture resources it needs
	ps->SetShaderResourceView("cubeMap", srvSky.Get());
	ps->SetSamplerState("samplerOptions", samplerOptions.Get());
	ps->CopyAllBufferData();

	// Draw the mesh
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, cube->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(cube->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	// Do the actual drawing
	context->DrawIndexed(
		cube->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

	// Reset rasterizer and depth state
	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);
}
