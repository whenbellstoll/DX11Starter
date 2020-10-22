#include "ShaderIncludes.hlsli"
// Assignment 3
// cbuffer
cbuffer ExternalData : register(b0)
{
	matrix proj;
	matrix view;
}


// Struct representing the data we're sending down the pipeline
// - Should match our pixel shader's input (hence the name: Vertex to Pixel)
// - At a minimum, we need a piece of data defined tagged as SV_POSITION
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// 
// - Input is exactly one vertex worth of data (defined by a struct)
// - Output is a single struct of data to pass down the pipeline
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
VertexToPixelSky main( VertexShaderInput input )
{
	// Set up output struct
	VertexToPixelSky output;


	// Create a version of the view matrix with NO translation
	matrix viewCopy = view;
	viewCopy._14 = 0; 
	viewCopy._24 = 0; 
	viewCopy._34 = 0; 

	// Calculate the output position
	matrix viewProj = mul(proj, viewCopy);
	output.position = mul(viewProj, float4(input.position, 1));

	// set z = w so that z is 1.0f
	output.position.z = output.position.w;

	// sample direction is the input position because position = offset from origin
	output.sampleDir = input.position;

	// Send to our pixel shader
	return output;
}