#include "Material.h"

Material::Material(DirectX::XMFLOAT4 color, Microsoft::WRL::ComPtr<ID3D11PixelShader> pS, Microsoft::WRL::ComPtr<ID3D11VertexShader> vS)
{
    colorTint = color;
    pixelShader = pS;
    vertexShader = vS;
}

DirectX::XMFLOAT4 Material::GetColorTint()
{
    return colorTint;
}

Microsoft::WRL::ComPtr<ID3D11PixelShader> Material::GetPixelShader()
{
    return pixelShader;
}

Microsoft::WRL::ComPtr<ID3D11VertexShader> Material::GetVertexShader()
{
    return vertexShader;
}
