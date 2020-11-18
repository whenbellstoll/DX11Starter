#include "ShaderIncludes.hlsli"


TextureCube cubeMap : register(t0);// "t" registers
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
float4 main(VertexToPixelSky input) : SV_TARGET
{
	float3 palette[12];

	palette[0] = float3(0, 0, 0);
	palette[1] = float3(0.0941f, 0.0941f, 0.0941f);
	palette[2] = float3(0.1882f, 0.1882f, 0.1882f);
	palette[3] = float3(0.2823f, 0.2823f, 0.2823f);
	palette[4] = float3(0.3764f, 0.3764f, 0.3764f);
	palette[5] = float3(0.4706f, 0.4706f, 0.4706f);
	palette[6] = float3(0.5647f, 0.5647f, 0.5647f);
	palette[7] = float3(0.6588f, 0.6588f, 0.6588f);
	palette[8] = float3(0.7529f, 0.7529f, 0.7529f);
	palette[9] = float3(0.8470f, 0.8470f, 0.8470f);
	palette[10] = float3(0.9412f, 0.9412f, 0.9412f);
	palette[11] = float3(1, 1, 1);

	float4 finalColor = cubeMap.Sample(samplerOptions, input.sampleDir);
	 int index = (int)((finalColor.r * 255.0f) / (255.0f / 12.0f));




	 if (index > 11)
		 index = 11;

	
	if (finalColor.b > 0.55f  && finalColor.r < 0.55f)
	{
		return float4(0, 1, 1, 0);
	}

	 if (index == 11)
	 {
		 if (finalColor.g < 0.5f)
		 {
			 return float4(1, 0, 0, 0);
		 }
	 }



	 return float4(palette[index], 0);
}