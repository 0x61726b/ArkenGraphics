//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//OmniDÝrectionalDepthVS.h
//--------------------------------------------------------------------------------
static const float DepthBias = 0.005f;
cbuffer TransformsDepth
{
	matrix gWorld;
	matrix gView;
	matrix WorldViewProjMatrix;
	matrix WorldInverseTranspose;
};
cbuffer cbNeverChange
{
	float4 LightRange;
	float4 LightColor;
	float4 LightPosWS;
};

TextureCube			   ShadowMap		: register(t0);
SamplerComparisonState ShadowMapSampler	: register(s0);
//--------------------------------------------------------------------------------
struct PSInput
{
	float4 PositionCS : SV_Position;
	float3 NormalWS : Attrib0;
	float3 LightDirWS : Attrib1;
	float3 ViewDirWS :  Attrib2;
};
static const float3 SamplingDisk[20] =
{
	float3(1,1,1),float3(1,-1,1),float3(-1,-1,1),float3(-1,1,1),
	float3(1,1,-1),float3(1,-1,-1),float3(-1,-1,-1),float3(-1,1,-1),
	float3(1,1,0),float3(1,-1,0),float3(-1,-1,0),float3(-1,1,0),
	float3(1,0,1),float3(-1,0,1),float3(1,0,-1),float3(-1,0,-1),
	float3(0,1,1),float3(0,-1,1),float3(0,-1,-1),float3(0,1,-1)
};
//--------------------------------------------------------------------------------
float SamplePCF(float3 lightDirWS)
{
	float3 light = lightDirWS;
	float lightDistance = length(light);
	light /= lightDistance;

	float fShadow = 0.0f;
	int numSamples = 0;

	const int Radius = 2;
	const float ShadowMapSize = 2048;
	const int NumSamples = (Radius*2 + 1) * (Radius*2 + 1);

	float3 fracs = frac(light.xyz * ShadowMapSize);
		float leftEdge = 1.0f - fracs.x;
	float rightEdge = fracs.x;
	float topEdge = 1.0f - fracs.y;
	float bottomEdge = fracs.y;
	float InnerEdge = 1.0f - fracs.z;


	[unroll(NumSamples)]
	for(int y = -Radius; y <= Radius; y++)
	{
		[unroll(NumSamples)]
		for(int x = -Radius; x <= Radius; x++)
		{
			float3 offset = float3(x,y,0.0001f) * (1.0f / ShadowMapSize);
				float3 sampleCoord = -light + offset;
				float newDistance = length(sampleCoord);
			float sample = ShadowMap.SampleCmp(ShadowMapSampler,sampleCoord,lightDistance/LightRange.x - DepthBias);

			fShadow += sample;
		}
	}
	fShadow /= NumSamples;
	fShadow *= 1.5f;
	return fShadow;
}
//--------------------------------------------------------------------------------
float4 PSMAIN(in PSInput input): SV_Target
{
	float4 LightAmbient = float4(1,0,1,1.0f);
	float4 LightDiffuse = float4(1.0f,0,0,1.0f);
	float4 LightSpecular = float4(1.0f,0.8f,0.0f,1.0f);

	float4 MaterialAmbient = float4(1.0f,0.0f,1.0f,1.0f);
	float4 MaterialDiffuse = float4(1,0,0,1.0f);
	float4 MaterialSpecular = float4(0.4f,0.4f,0.4f,1.0f);

	float3 light = input.LightDirWS;
	float lightDistance = length(light);
	light /= lightDistance;
	

	float3 view = normalize(input.ViewDirWS);
	float3 normal = normalize(input.NormalWS);
	float3 reflection = reflect(-light,normal);

	float4 ambientIntensity = float4(0.1f,0.1f,0.1f,1);
	float lightAttenuation = 0.02f;

	float diffuseFactor = max(dot(light,normal),0.0f);
	float4 diffuse = lightAttenuation*diffuseFactor*float4(0,1,1,1);

		float specularFactor = pow(max(dot(reflection,view),0.0f),50.0f);
	float4 specularIntensity  = lightAttenuation * specularFactor * (LightSpecular * MaterialSpecular);
		if(dot(light,normal) < 0.0f)
			specularIntensity	  = float4(0.0f,0.0f,0.0f,1.0f);

	float fShadow = SamplePCF(input.LightDirWS);
	
	float shadowFactor		  = ShadowMap.SampleCmp(ShadowMapSampler,-light,lightDistance/LightRange.x - DepthBias);

	
	return ambientIntensity + (diffuse+specularIntensity)*fShadow;
	/*return float4(normal,1.0f);*/
}