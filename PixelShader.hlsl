
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
};

struct DirectionalLight
{
	float3 ambientColor;
	float3 diffuseColor;
	float3 direction;
};

cbuffer ExternalData : register(b0)
{
	DirectionalLight directionalLight;
	DirectionalLight lightTwo;
	DirectionalLight lightThree;
}

// helper Method
float3 directionalLightCalculation( VertexToPixel i, DirectionalLight dl)
{
	float3 iNormal = normalize(i.normal);
	float3 toTheLight = normalize(-(dl.direction));
	toTheLight = dot(toTheLight, iNormal);
	float3 finalColor = toTheLight * dl.diffuseColor * ((float3)i.color + dl.ambientColor) * (float3)i.color;
	finalColor = saturate(finalColor);
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
	float3 lOne = directionalLightCalculation(input, directionalLight);
	float3 lTwo = directionalLightCalculation(input, lightTwo);
	float3 lThree = directionalLightCalculation(input, lightThree);
	float3 finalColor = lOne + lTwo + lThree;
	finalColor = saturate(finalColor);
	return float4(finalColor, 1);
}