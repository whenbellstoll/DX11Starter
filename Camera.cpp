#include "Camera.h"

Camera::Camera(DirectX::XMFLOAT3 pos, float nFov, float nearClip, float farClip)
{
    transform.SetPosition(pos.x, pos.y, pos.z);
    fov = nFov;
    nearPlaneDistance = nearClip;
    farPlaneDistance = farClip;
    mousePos = POINT();
    XMStoreFloat4x4(&projMatrix, DirectX::XMMatrixIdentity());
    XMStoreFloat4x4(&viewMatrix, DirectX::XMMatrixIdentity());
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
    return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjMatrix()
{
    return projMatrix;
}

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
    XMStoreFloat4x4(&projMatrix, DirectX::XMMatrixPerspectiveFovLH(transform.GetRotation().y * (0.017453292f), aspectRatio, nearPlaneDistance, farPlaneDistance) );
}

void Camera::UpdateViewMatrix()
{
    DirectX::XMVECTOR positionVec = DirectX::XMVectorSet(transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z, 0);
    
    // Direction Vector
    DirectX::XMVECTOR worldForward = DirectX::XMVectorSet(0, 0, 1, 0);
    DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(transform.GetRotation().x, transform.GetRotation().y, transform.GetRotation().z);
    DirectX::XMVECTOR directionVec = DirectX::XMVector3Rotate(worldForward, quaternion);

    DirectX::XMVECTOR upVec = DirectX::XMVectorSet(0, 1, 0, 0);

    XMStoreFloat4x4(&viewMatrix, DirectX::XMMatrixLookToLH( positionVec, directionVec, upVec) );
}

void Camera::Update(float dt, HWND windowHandle)
{
}

