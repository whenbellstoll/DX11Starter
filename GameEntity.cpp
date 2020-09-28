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

void GameEntity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, UINT stride, UINT offset, Camera * camera )
{
	material->GetVertexShader()->SetShader();
	material->GetPixelShader()->SetShader();

	// Vertex Shader data struct
	SimpleVertexShader* vs = material->GetVertexShader(); // Simplifies next few lines
	vs->SetFloat4("colorTint", material->GetColorTint());
	vs->SetMatrix4x4("world", transform.GetWorldMatrix());
	vs->SetMatrix4x4("view", camera->GetViewMatrix() );
	vs->SetMatrix4x4("proj", camera->GetProjMatrix() );

	// Map the buffer data
	vs->CopyAllBufferData();


	// Set the buffer
	context->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	// Do the actual drawing
	context->DrawIndexed(
		mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}