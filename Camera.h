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

	// Constructor
	Camera(DirectX::XMFLOAT3 pos, float nFov, float nearClip, float far Clip);

	// Getters
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjMatrix();

	void UpdateProjectionMatrix( float aspectRatio );
	void UpdateViewMatrix();

	void Update(float dt, HWND windowHandle);
};

