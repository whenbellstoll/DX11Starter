#include "ShaderIncludes.hlsli"
cbuffer ExternalData : register(b0)
{
	DirectionalLight directionalLight;
	DirectionalLight lightTwo;
	DirectionalLight lightThree;
	DirectionalLight pointLight;
	float specularValue;
	float specularExpo;
	float3 cameraPosition;
	
	
}

Texture2D diffuseTexture : register(t0);// "t" registers
Texture2D normalMap : register(t1);
SamplerState samplerOptions: register(s0);// "s" registers


// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixelNormalMap input) : SV_TARGET
{
	// calculate normal map and surface color from textures
	float3 unpackedNormal = normalMap.Sample(samplerOptions, input.uv).rgb * 2 - 1;
	float3 surfaceColor = pow( diffuseTexture.Sample(samplerOptions, input.uv).rgb, 2.2f);

	//normalize the normal
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	float3 N = input.normal;
	float3 T = input.tangent;
	T = normalize(T - N * dot(T, N)); // Gram-Schmidt orthogonalization
	float3 B = cross(T, N); // Bi-tangent
	float3x3 TBN = float3x3(T, B, N);

	input.normal = normalize( mul(unpackedNormal, TBN) ); // order matters because matrix


	float3 finalColor = surfaceColor + directionalLightCalculationN(input, directionalLight, surfaceColor, cameraPosition, specularExpo);
	finalColor = finalColor + directionalLightCalculationN(input, lightTwo, surfaceColor, cameraPosition, specularExpo);
	finalColor = finalColor + directionalLightCalculationN(input, lightThree, surfaceColor, cameraPosition, specularExpo);
	finalColor = finalColor + pointLightCalculationN(input, pointLight, surfaceColor, cameraPosition, specularExpo);

	// ambient light
	float3 ambientLight = directionalLight.ambientColor;
	finalColor = finalColor + ambientLight;

	//return float4(finalColor, 1);
	//stylization
	float4 palette[12];

	palette[0] = float4(0, 0, 0, 1);
	palette[1] = float4(0.0941f, 0.0941f, 0.0941f, 1);
	palette[2] = float4(0.1882f, 0.1882f, 0.1882f, 1);
	palette[3] = float4(0.2823f, 0.2823f, 0.2823f, 1);
	palette[4] = float4(0.3764f, 0.3764f, 0.3764f, 1);
	palette[5] = float4(0.4706f, 0.4706f, 0.4706f, 1);
	palette[6] = float4(0.5647f, 0.5647f, 0.5647f, 1);
	palette[7] = float4(0.6588f, 0.6588f, 0.6588f, 1);
	palette[8] = float4(0.7529f, 0.7529f, 0.7529f, 1);
	palette[9] = float4(0.8470f, 0.8470f, 0.8470f, 1);
	palette[10] = float4(0.9412f, 0.9412f, 0.9412f, 1);
	palette[11] = float4(1, 1, 1, 1);

	int index = (int)((finalColor.r * 255.0f) / (255.0f / 12.0f));




	if (index > 8)
		index -= 1;

	if (index > 11)
		index = 11;

	float3 finalHSV = rgbhsv(finalColor);

	if (finalHSV.r > 150 && finalHSV.r < 240 && finalHSV.g > 0.20f)
	{
		return float4(0, 1, 1, 1);
	}



	if ((finalHSV.r > 335 || finalHSV.r < 20) && finalHSV.g > 0.75f)
	{
		return float4(1, 0, 0, 1);
	}




	return float4(palette[index]);

	
}