#pragma once
#include "Transform.h"
#include "Mesh.h"
#include <DirectXMath.h>
class GameEntity
{
	Transform transform;
	Mesh* mesh;

	// Constructor
	GameEntity(Mesh* m);

	// Getters
	Mesh* GetMesh();
	Transform* GetTransform();

	// Draw Method
	void Draw();
};

