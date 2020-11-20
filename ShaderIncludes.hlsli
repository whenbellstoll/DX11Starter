#ifndef __GGP_SHADER_INCLUDES__
#define __GGP_SHADER_INCLUDES__
// ALL of your code pieces (structs, functions, etc.) gohere!

// PBR Constants
// http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
static const float  F0_NON_METAL = 0.04f;

// Minimum roughness for when spec distribution function denominator goes to zero
static const float MIN_ROUGHNESS = 0.0000001f; 

// Handy to have this as a constant
static const float PI = 3.14159265359f;


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


// PBR Functions
// Lambert diffuse BRDF - Same as the basic lighting diffuse calculation!
// - NOTE: this function assumes the vectors are already NORMALIZED!
float DiffusePBR(float3 normal, float3 toTheLight)
{
	return saturate(dot(normal, toTheLight));
}

// Calculates diffuse amount based on energy conservation
// diffuse - Diffuse amount
// specular - Specular color (including light color)
// metalness - surface metalness amount 
// Metals should have an albedo of (0,0,0)...mostly
// See slide 65: http://blog.selfshadow.com/publications/s2014-shading-course/hoffman/s2014_pbs_physics_math_slides.pdf 
float3 DiffuseEnergyConserve(float diffuse, float3 specular, float metalness)
{
	return diffuse * ((1 - saturate(specular)) * (1 - metalness));
}

// GGX (Trowbridge-Reitz)
// a - Roughness 
// h - Half vector
// n - Normal 
// D(h, n) = a^2 / pi * ((n dot h)^2 * (a^2 - 1) + 1)^2
float SpecDistribution(float3 n, float3 h, float roughness)
{
	// Pre-calculations
	float NdotH = saturate(dot(n, h));
	float NdotH2 = NdotH * NdotH;
	float a = roughness * roughness; 
	float a2 = max(a * a, MIN_ROUGHNESS); 
	// Applied after remap! 
	// ((n dot h)^2 * (a^2 - 1) + 1)
	float denomToSquare = NdotH2 * (a2 - 1) + 1;
	// Can go to zero if roughness is 0 and NdotH is 1 
	// Final value 
	return a2 / (PI * denomToSquare * denomToSquare);
}

// Fresnel term - Schlick approx. 
// v - View vector 
// h - Half vector
// f0 - Value when l = n (full specular color) 
// F(v,h,f0) = f0 + (1-f0)(1 - (v dot h))^5
float3 Fresnel(float3 v, float3 h, float3 f0)
{
	// Pre-calculations
	float VdotH = saturate(dot(v, h));
	// Final value 
	return f0 + (1 - f0) * pow(1 - VdotH, 5);
}

// Geometric Shadowing - Schlick-GGX (based on Schlick-Beckmann) 
// - k is remapped to a / 2, roughness remapped to (r+1)/2 
// n - Normal 
// v - View vector 
// G(l,v,h) 
float GeometricShadowing(float3 n, float3 v, float3 h, float roughness)
{
	// End result of remapping: 
	float k = pow(roughness + 1, 2) / 8.0f;
	float NdotV = saturate(dot(n, v));
	
	// Final value
	return NdotV / (NdotV * (1 - k) + k);
}

// Microfacet BRDF (Specular) 
// f(l,v) = D(h)F(v,h)G(l,v,h) / 4(n dot l)(n dot v) 
// - part of the denominator are canceled out by numerator (see below) 
// D() - Spec Dist - Trowbridge-Reitz (GGX) 
// F() - Fresnel - Schlick approx// G() - Geometric Shadowing - Schlick-GGX 
float3 MicrofacetBRDF(float3 n, float3 l, float3 v, float roughness, float metalness, float3 specColor)
{
	// Other vectors
	float3 h = normalize(v + l);
	// Grab various functions 
	float D = SpecDistribution(n, h, roughness); 
	float3 F = Fresnel(v, h, specColor); 
	float G = GeometricShadowing(n, v, h, roughness) * GeometricShadowing(n, l, h, roughness); 
	
	// Final formula 
	// Denominator dot products partially canceled by G()! 
	// See page 16: http://blog.selfshadow.com/publications/s2012-shading-course/hoffman/s2012_pbs_physics_math_notes.pdf 
	return (D * F * G) / (4 * max(dot(n,v), dot(n,l)));
}

// HSV from RGB formula (use sparingly)
float3 rgbhsv(float3 rgb)
{
	float3 hsv;
	float min, max, delta;

	min = rgb.r < rgb.g ? rgb.r : rgb.g;
	min = min < rgb.b ? min : rgb.b;

	max = rgb.r > rgb.g ? rgb.r : rgb.g;
	max = max > rgb.b ? max : rgb.b;

	hsv.b = max;                                // v
	delta = max - min;
	if (delta < 0.00001)
	{
		hsv.g = 0;
		hsv.r = 0; // undefined, maybe nan?
		return hsv;
	}
	if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
		hsv.g = (delta / max);                  // s
	}
	else {
		// if max is 0, then r = g = b = 0              
		// s = 0, h is undefined
		hsv.g = 0.0;
		hsv.r = 0.0;
		return hsv;
	}

	if (rgb.r >= max)
	{
		hsv.r = (rgb.g - rgb.b) / delta;        // between yellow & magenta
	}
	else
	{
		if (rgb.g >= max)
		{
			hsv.r = 2.0 + (rgb.b - rgb.r) / delta;  // between cyan & yellow
		}
		else
		{
			hsv.r = 4.0 + (rgb.r - rgb.g) / delta;  // between magenta & cyan
		}
	}
	hsv.r *= 60.0;                              // degrees

	if (hsv.r < 0.0)
		hsv.r += 360.0;

	return hsv;
}

#endif