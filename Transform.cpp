#include "Transform.h"

Transform::Transform()
{
	position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
}

DirectX::XMFLOAT3 Transform::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Transform::GetRotation()
{
	return rotation;
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return scale;
}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	// Update World Matrix
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation( position.x, position.y, position.z);
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX rpyMatrix = DirectX::XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z);

	// From the example, order is scale, rotation, translation in multiplcation.
	DirectX::XMMATRIX world = scaleMatrix * rpyMatrix * translationMatrix;
	
	// Put our calculation into our Float4x4
	DirectX::XMStoreFloat4x4(&worldMatrix, world);

	// return our 4x4
	return worldMatrix;
}


// setters
void Transform::SetPosition(float x, float y, float z)
{
	position = DirectX::XMFLOAT3(x, y, z);
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
	rotation = DirectX::XMFLOAT3(pitch, yaw, roll);
}

void Transform::SetScale(float x, float y, float z)
{
	scale = DirectX::XMFLOAT3(x, y, z);
}


// Transformers
void Transform::MoveAbsolute(float x, float y, float z)
{
	//just add to the current position :)
	position = DirectX::XMFLOAT3(position.x + x, position.y + y, position.z + z);
}

void Transform::Rotate(float pitch, float yaw, float roll)
{
	rotation = DirectX::XMFLOAT3(rotation.x + pitch, rotation.y + yaw, rotation.z + roll);
}

void Transform::Scale(float x, float y, float z)
{
	scale = DirectX::XMFLOAT3( scale.x + x, scale.y + y, scale.z + z );
}

void Transform::MoveRelative(float x, float y, float z)
{
	// Direction Vector
	DirectX::XMVECTOR absoluteDirection = DirectX::XMVectorSet(x, y, z, 0);
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	DirectX::XMVECTOR directionVec = DirectX::XMVector3Rotate(absoluteDirection, quaternion);

	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&GetPosition());
	pos = DirectX::XMVectorAdd(pos, directionVec);
	DirectX::XMStoreFloat3(&position, pos);
}