#ifndef __GGP_SHADER_INCLUDES__
#define __GGP_SHADER_INCLUDES__
// ALL of your code pieces (structs, functions, etc.) gohere!

// Transition struct between Vertex and Pixel shaders
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

//Transition struct for normal map shaders
struct VertexToPixelNormalMap
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
	float3 tangent		: TANGENT;
};

// Transition struct for cube maps
struct VertexToPixelSky
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 sampleDir	: SAMPLEDIRECTION;
};

// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 position		: POSITION;     // XYZ position     
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
};


struct DirectionalLight
{
	float3 ambientColor;
	float3 diffuseColor;
	float3 direction;
	int type;
};

// Lighting Helper Methods
float specCalculation(VertexToPixel i, float3 direction, float3 cameraPosition, float specExpo)
{
	float3 V = normalize(cameraPosition - (float3)i.worldPos);
	float3 R = reflect(direction, i.normal);
	return pow(saturate(dot(R, V)), specExpo);
}

float3 directionalLightCalculation(VertexToPixel i, DirectionalLight dl, float3 surfaceColor, float3 cameraPosition, float specExpo)
{
	float3 iNormal = i.normal;
	float3 toTheLight = normalize(dl.direction);
	float diffuse = saturate(dot(iNormal, toTheLight));
	float spec = specCalculation(i, -toTheLight, cameraPosition, specExpo);
	return (diffuse * dl.diffuseColor * surfaceColor + spec * dl.diffuseColor);

}

float3 pointLightCalculation(VertexToPixel i, DirectionalLight dl, float3 surfaceColor, float3 cameraPosition, float specExpo)
{
	// Direction of a point light = position
	float3 iNormal = i.normal;
	float3 toTheLight = normalize(dl.direction - (float3)i.worldPos);
	float diffuse = saturate(dot(toTheLight, iNormal));
	float spec = specCalculation(i, -toTheLight, cameraPosition, specExpo);
	return (diffuse * dl.diffuseColor * surfaceColor + spec * dl.diffuseColor);
}


float specCalculationN(VertexToPixelNormalMap i, float3 direction, float3 cameraPosition, float specExpo)
{
	float3 V = normalize(cameraPosition - (float3)i.worldPos);
	float3 R = reflect(direction, i.normal);
	return pow(saturate(dot(R, V)), specExpo);
}

float3 directionalLightCalculationN(VertexToPixelNormalMap i, DirectionalLight dl, float3 surfaceColor, float3 cameraPosition, float specExpo)
{
	float3 iNormal = i.normal;
	float3 toTheLight = normalize(dl.direction);
	float diffuse = saturate(dot(iNormal, toTheLight));
	float spec = specCalculationN(i, -toTheLight, cameraPosition, specExpo);
	spec *= any(diffuse);
	return (diffuse * dl.diffuseColor * surfaceColor + spec * dl.diffuseColor);

}

float3 pointLightCalculationN(VertexToPixelNormalMap i, DirectionalLight dl, float3 surfaceColor, float3 cameraPosition, float specExpo)
{
	// Direction of a point light = position
	float3 iNormal = i.normal;
	float3 toTheLight = normalize(dl.direction - (float3)i.worldPos);
	float diffuse = saturate(dot(toTheLight, iNormal));
	float spec = specCalculationN(i, -toTheLight, cameraPosition, specExpo);
	spec *= any(diffuse);
	return (diffuse * dl.diffuseColor * surfaceColor + spec * dl.diffuseColor);
}

#endif