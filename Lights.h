#pragma once
#include <DirectXMath.h>

struct DirectionalLight
{
	DirectX::XMFLOAT3 ambientColor;
	float paddingOne;
	DirectX::XMFLOAT3 diffuseColor;
	float paddingTwo;
	DirectX::XMFLOAT3 direction;
	int type;
};