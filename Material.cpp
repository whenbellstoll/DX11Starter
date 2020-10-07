#include "Material.h"

Material::Material(DirectX::XMFLOAT4 color, SimplePixelShader* pS, SimpleVertexShader* vS, float specularV, float specularE, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sr, Microsoft::WRL::ComPtr<ID3D11SamplerState> samS)
{
    colorTint = color;
    pixelShader = pS;
    vertexShader = vS;
    specularValue = specularV;
    specularExpo = specularE;
    srv = sr;
    sampleState = samS;
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

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetSRV()
{
    return srv;
}

Microsoft::WRL::ComPtr<ID3D11SamplerState> Material::GetSampleState()
{
    return sampleState;
}

float Material::GetSpec()
{
    return specularValue;
}

float Material::GetSpecExpo()
{
    return specularExpo;
}