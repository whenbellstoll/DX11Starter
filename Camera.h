#pragma once
#include <Windows.h>
#include "Transform.h"
class Camera
{
	// Fields
	Transform transform;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projMatrix;
	POINT mousePos;
	float fov;
	float nearPlaneDistance;
	float farPlaneDistance;

};

