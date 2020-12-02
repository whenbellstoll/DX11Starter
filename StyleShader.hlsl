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
	float4 palette[12];
	int silhouetteID;
	
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


	// really scholcked implementation until I can figure out one that can handle an arbitrary palette
	// stylize the color based on external data
	int index = (int)((finalColor.r * 255.0f) / (255.0f / 12.0f));


	

	if (index > 8)
		index -= 1;

	if (index > 11)
		index = 11;
	
	// if the palette is the grayscale palette
	if (palette[1].r - 0.0941f < 0.005f && palette[1].r - 0.0941f > -0.005f)
	{
		float3 finalHSV = rgbhsv(finalColor);

		if (finalHSV.r > 150 && finalHSV.r < 240 && finalHSV.g > 0.20f)
		{
			return float4(0, 1, 1, silhouetteID / 256.0f);
		}



		if ((finalHSV.r > 335 || finalHSV.r < 20) && finalHSV.g > 0.75f)
		{
			return float4(1, 0, 0, silhouetteID / 256.0f);
		}

	}
	
	return float4( palette[index].r, palette[index].g, palette[index].b, silhouetteID / 256.0f);
}