#include "GameEntity.h"

GameEntity::GameEntity( Mesh * m , Material * mat)
{
	mesh = m;
	material = mat;
}

Mesh* GameEntity::GetMesh()
{
	return mesh;
}

Transform* GameEntity::GetTransform()
{
	return &transform;
}

void GameEntity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Buffer> buffer, UINT stride, UINT offset, Camera * camera )
{
	context->VSSetShader( material->GetVertexShader().Get(), 0, 0);
	context->PSSetShader( material->GetPixelShader().Get(), 0, 0);

	// Vertex Shader data struct
	VertexShaderExternalData vsData;
	vsData.colorTint = material->GetColorTint();
	vsData.worldMatrix = transform.GetWorldMatrix();
	vsData.projMatrix = camera->GetProjMatrix();
	vsData.viewMatrix = camera->GetViewMatrix();

	// Map the buffer data
	D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
	context->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));
	context->Unmap(buffer.Get(), 0);


	// Bind our constant Buffer
	context->VSSetConstantBuffers(0, // Which slot (register) to bind the buffer to?
		1,// How many are we activating?  Can do multiple at once 
		buffer.GetAddressOf());// Array of buffers (or the address of one)

	// Set the buffer
	context->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	// Do the actual drawing
	context->DrawIndexed(
		mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}