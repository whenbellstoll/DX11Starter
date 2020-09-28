#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "BufferStructs.h"
#include "Camera.h"
#include "Material.h"
#include <DirectXMath.h>
class GameEntity
{
	Transform transform;
	Mesh* mesh;
	Material* material;

public:
	// Constructor
	GameEntity(Mesh* m, Material * mat);

	// Getters
	Mesh* GetMesh();
	Transform* GetTransform();

	// Draw Method
	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, UINT stride, UINT offset, Camera * camera);
};

