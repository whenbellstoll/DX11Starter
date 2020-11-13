#include "Material.h"

Material::Material(DirectX::XMFLOAT4 color, SimplePixelShader* pS, SimpleVertexShader* vS, float specularV, float specularE, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sr, Microsoft::WRL::ComPtr<ID3D11SamplerState> samS)
{
    colorTint = color;
    pixelShader = pS;
    vertexShader = vS;
    specularValue = specularV;
    specularExpo = specularE;
    srv = sr;
    roughMap = sr;
    metalMap = sr;
    sampleState = samS;
    normalMapBool = false;
    PBRBool = false;
}

Material::Material(DirectX::XMFLOAT4 color, SimplePixelShader* pS, SimpleVertexShader* vS, float specularV, float specularE, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sr, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> nm, Microsoft::WRL::ComPtr<ID3D11SamplerState> samS)
{
    colorTint = color;
    pixelShader = pS;
    vertexShader = vS;
    specularValue = specularV;
    specularExpo = specularE;
    srv = sr;
    sampleState = samS;
    normalMap = nm;
    normalMapBool = true;
    PBRBool = false;
}

Material::Material(DirectX::XMFLOAT4 color, SimplePixelShader* pS, SimpleVertexShader* vS, float specularV, float specularE, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sr, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> nm, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> rm, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mm, Microsoft::WRL::ComPtr<ID3D11SamplerState> samS)
{
    colorTint = color;
    pixelShader = pS;
    vertexShader = vS;
    specularValue = specularV;
    specularExpo = specularE;
    srv = sr;
    sampleState = samS;
    normalMap = nm;
    roughMap = rm;
    metalMap = mm;
    normalMapBool = true;
    PBRBool = true;
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

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetNormal()
{
    return normalMap;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetRough()
{
    return roughMap;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetMetal()
{
    return metalMap;
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

bool Material::IsNormal()
{
    return normalMapBool;
}

bool Material::IsPBR()
{
    return PBRBool;
}