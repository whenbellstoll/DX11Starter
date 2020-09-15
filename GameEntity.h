#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "BufferStructs.h"
#include <DirectXMath.h>
class GameEntity
{
	Transform transform;
	Mesh* mesh;

public:
	// Constructor
	GameEntity(Mesh* m);

	// Getters
	Mesh* GetMesh();
	Transform* GetTransform();

	// Draw Method
	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Buffer> buffer, UINT stride, UINT offset);
};

