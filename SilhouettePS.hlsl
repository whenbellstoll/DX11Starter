cbuffer ExternalData : register(b0)
{
	float pixelWidth;
	float pixelHeight;
	float4 palette[12];
}


// Defines the input to this pixel shader
struct VertexToSilho
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

// Textures and such
Texture2D pixels			: register(t0);
SamplerState samplerOptions	: register(s0);


// rgb to hsv function. expensive. use sparingly.
float3 rgbhsv(float3 rgb)
{
	float3 hsv = float3(0, 0, 0);
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
		hsv.r = 0; // undefined, maybe nan? decided on 0
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


float4 main(VertexToSilho input) : SV_TARGET
{
	
	// Take 9 samples from the texture
	float4 samples[9];
	samples[0] = pixels.Sample(samplerOptions, input.uv + float2(-pixelWidth, -pixelHeight));
	samples[1] = pixels.Sample(samplerOptions, input.uv + float2(0, -pixelHeight));
	samples[2] = pixels.Sample(samplerOptions, input.uv + float2(pixelWidth, -pixelHeight));
	samples[3] = pixels.Sample(samplerOptions, input.uv + float2(-pixelWidth, 0));
	samples[4] = pixels.Sample(samplerOptions, input.uv); 
	samples[5] = pixels.Sample(samplerOptions, input.uv + float2(pixelWidth, 0));
	samples[6] = pixels.Sample(samplerOptions, input.uv + float2(-pixelWidth, pixelHeight));
	samples[7] = pixels.Sample(samplerOptions, input.uv + float2(0, pixelHeight));
	samples[8] = pixels.Sample(samplerOptions, input.uv + float2(pixelWidth, pixelHeight));

	// Compare the IDs of surrounding pixels to the ID of this pixel (samples[4])
	bool same =
		samples[0].a == samples[4].a &&
		samples[1].a == samples[4].a &&
		samples[2].a == samples[4].a &&
		samples[3].a == samples[4].a &&
		samples[5].a == samples[4].a &&
		samples[6].a == samples[4].a &&
		samples[7].a == samples[4].a &&
		samples[8].a == samples[4].a;
	
	// We know that this pixel is an outline
	if( !same )
	return float4(0, 0, 0, 1);

	// We know that it is an inner pixel, do the pallette shader post process
	// stylize the color based on the current pixel
	int index = (int)((samples[4].r * 255.0f) / (255.0f / 12.0f));



	if (index > 11)
		index = 11;


	// NOT FUN! ...  OR GOOD!
	// if the palette is the grayscale palette
	if (palette[1].r - 0.0941f < 0.005f && palette[1].r - 0.0941f > -0.005f)
	{
		float3 finalHSV = rgbhsv(samples[4]);

		if (finalHSV.r > 150 && finalHSV.r < 240 && finalHSV.g > 0.30f)
		{
			return float4(0, 1, 1, 0);
		}

		if ((finalHSV.r > 335 || finalHSV.r < 20) && finalHSV.g > 0.55f)
		{
			return float4(1, 0, 0, 0);
		}
	}

	return float4(palette[index]);
}