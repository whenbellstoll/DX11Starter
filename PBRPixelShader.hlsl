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
	int silhouetteID;
}

// Texture-related variables
Texture2D AlbedoTexture			: register(t0);
Texture2D RoughnessTexture		: register(t2);
Texture2D MetalTexture			: register(t3);
Texture2D normalMap : register(t1);
SamplerState samplerOptions: register(s0); // "s" registers


float3 PBRLightCalculation(DirectionalLight light, float3 normal, float3 toLight, float3 toCam, float roughness, float metalness, float3 specColor, float4 surfaceColor)
{
	// Calculate the light amounts
	float diff = DiffusePBR(normal, toLight);
	float3 spec = MicrofacetBRDF(normal, toLight, toCam, roughness, metalness, specColor);
	// Calculate diffuse with energy conservation
	// (Reflected light doesn't get diffused)
	float3 balancedDiff = DiffuseEnergyConserve(diff, spec, metalness);
	// Combine the final diffuse and specular values for this light
	return (balancedDiff * (float3)surfaceColor + spec) * light.diffuseColor;
}

// Entry point for this pixel shader
float4 main(VertexToPixelNormalMap input) : SV_TARGET
{
	// Fix for poor normals: re-normalizing interpolated normals
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	// Sample texture
	float4 albedoColor = AlbedoTexture.Sample(samplerOptions, input.uv);
	albedoColor.rgb = pow(albedoColor.rgb, 2.2);

	// Unpack normal map
	float3 unpackedNormal = normalMap.Sample(samplerOptions, input.uv).rgb * 2 - 1;

	//Calculate Normal Map
	float3 N = input.normal;
	float3 T = input.tangent;
	T = normalize(T - N * dot(T, N)); // Gram-Schmidt orthogonalization
	float3 B = cross(T, N); // Bi-tangent
	float3x3 TBN = float3x3(T, B, N);

	input.normal = normalize(mul(unpackedNormal, TBN)); // order matters because matrix

	// Sample the roughness map
	float roughness = RoughnessTexture.Sample(samplerOptions, input.uv).r;

	// Sample the metal map
	float metal = MetalTexture.Sample(samplerOptions, input.uv).r;

	
	// Specular color - Assuming albedo texture is actually holding specular color if metal == 1
	// Note the use of lerp here - metal is generally 0 or 1, but might be in between
	// because of linear texture sampling, so we want lerp the specular color to match
	float3 specColor = lerp(F0_NON_METAL.rrr, albedoColor.rgb, metal);


	//To the Camera
	float3 toCam = normalize(cameraPosition - (float3)input.worldPos);

	// Total color for this pixel
	float3 totalColor = float3(0,0,0);
	totalColor += PBRLightCalculation(directionalLight, input.normal, normalize(directionalLight.direction), toCam, roughness, metal, specColor, albedoColor);
	totalColor += PBRLightCalculation(lightThree, input.normal, normalize(lightThree.direction), toCam, roughness, metal, specColor, albedoColor);
	totalColor += PBRLightCalculation(lightTwo, input.normal, normalize(lightTwo.direction), toCam, roughness, metal, specColor, albedoColor);
	totalColor += PBRLightCalculation(pointLight, input.normal, normalize(pointLight.direction - (float3)input.worldPos), toCam, roughness, metal, specColor, albedoColor);

	// Adjust the light color by the light amount
	float3 final = pow(totalColor, 1.0f / 2.2f);

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

	int index = (int)((final.r * 255.0f) / (255.0f / 12.0f));




	if (index > 8)
		index -= 1;

	if (index > 11)
		index = 11;

	float3 finalHSV = rgbhsv(final);

	if (finalHSV.r > 150 && finalHSV.r < 240 && finalHSV.g > 0.20f)
	{
		return float4(0, 1, 1, silhouetteID / 256.0f);
	}



	if ((finalHSV.r > 335 || finalHSV.r < 20) && finalHSV.g > 0.75f)
	{
		return float4(1, 0, 0, silhouetteID / 256.0f);
	}



	palette[index].a = silhouetteID / 256.0f;
	return float4(palette[index]);

}