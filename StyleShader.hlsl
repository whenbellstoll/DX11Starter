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


	// really scholcked implementation until I can figure out one that can handle an arbitrary palette
	// stylize the color based on external data
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



	if ( (finalHSV.r > 335 || finalHSV.r < 20 ) && finalHSV.g > 0.55f)
	{
		return float4(1, 0, 0, 1);
	}
	

	

	return float4( palette[index] );
}