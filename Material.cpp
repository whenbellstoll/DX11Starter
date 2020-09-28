#include "Material.h"

Material::Material(DirectX::XMFLOAT4 color, SimplePixelShader* pS, SimpleVertexShader* vS)
{
    colorTint = color;
    pixelShader = pS;
    vertexShader = vS;
}

DirectX::XMFLOAT4 Material::GetColorTint()
{
    return colorTint;
}

SimplePixelShader* Material::GetPixelShader()
{
    return pixelShader;
}

SimpleVertexShader* Material::GetVertexShader()
{
    return vertexShader;
}
