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
float4 main(VertexToPixel input) : SV_TARGET
{

	float3 surfaceColor = diffuseTexture.Sample(samplerOptions, input.uv).rgb;

	//normalize the normal
	input.normal = normalize(input.normal);

	float3 finalColor = surfaceColor + directionalLightCalculation(input, directionalLight, surfaceColor, cameraPosition, specularExpo);
	finalColor = finalColor + directionalLightCalculation(input, lightTwo, surfaceColor, cameraPosition, specularExpo);
	finalColor = finalColor + directionalLightCalculation(input, lightThree, surfaceColor, cameraPosition, specularExpo);
	finalColor = finalColor + pointLightCalculation(input, pointLight, surfaceColor, cameraPosition, specularExpo);

	// ambient light
	float3 ambientLight = directionalLight.ambientColor;
	finalColor = finalColor + ambientLight;

	return float4(finalColor, 1);
}