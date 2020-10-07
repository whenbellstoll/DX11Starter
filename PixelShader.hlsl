
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
	float4 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};

struct DirectionalLight
{
	float3 ambientColor;
	float3 diffuseColor;
	float3 direction;
	int type;
};

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


// helper Method
float specCalculation(VertexToPixel i, float3 direction)
{
	float3 V = normalize(cameraPosition - (float3)i.worldPos);
	float3 R = reflect(direction, i.normal);
	float spec = pow(saturate(dot(R, V)), specularExpo) * specularValue;
	return spec;
}

float3 directionalLightCalculation( VertexToPixel i, DirectionalLight dl)
{
	float3 iNormal = normalize(i.normal);
	float3 toTheLight = normalize(-(dl.direction));
	toTheLight = dot(toTheLight, iNormal);
	float3 finalColor = dl.diffuseColor * saturate(toTheLight);
	finalColor = finalColor * specCalculation(i, toTheLight);
	return finalColor;
}

float3 pointLightCalculation(VertexToPixel i, DirectionalLight dl)
{
	// Direction of a point light = position
	float3 iNormal = normalize(i.normal);
	float3 toTheLight = dl.direction - (float3)i.worldPos;
	toTheLight = dot(toTheLight, iNormal);
	float3 finalColor = dl.diffuseColor * saturate(toTheLight);
	finalColor = finalColor * specCalculation(i, toTheLight);
	return finalColor;
}



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
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	float3 surfaceColor = diffuseTexture.Sample(samplerOptions, input.uv).rgb;

	float3 finalColor = surfaceColor + directionalLightCalculation(input, directionalLight);
	finalColor = finalColor + directionalLightCalculation(input, lightTwo);
	finalColor = finalColor + directionalLightCalculation(input, lightThree);
	finalColor = finalColor + pointLightCalculation(input, pointLight);

	// ambient light
	float3 ambientLight = (directionalLight.ambientColor + lightTwo.ambientColor + lightThree.ambientColor) / 3;
	finalColor = finalColor + ambientLight;

	return float4(finalColor, 1);
}