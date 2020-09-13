#pragma once
#include <DirectXMath.h>

class Transform
{
	// Fields
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4X4 worldMatrix;

	// Constructor
	Transform();

	// Getters
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT4X4 GetWorldMatrix();

	// Setters
	void SetPosition(float x, float y, float z); 
	void SetRotation(float pitch, float yaw, float roll);
	void SetScale(float x, float y, float z);

	// Transformers
	void MoveAbsolute(float x, float y, float z);
	void Rotate(float pitch, float yaw, float roll);
	void Scale(float x, float y, float z);
};

