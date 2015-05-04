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
static const float3 Poisson[4] =
{
	float3(-0.94201624,-0.39906216,-0.234124),
	float3(0.94558609,-0.76890725,-0.234124),
	float3(-0.094184101,-0.92938870,-0.234124),
	float3(0.34495938,0.29387760,-0.234124)
};
//--------------------------------------------------------------------------------
float SamplePCF()
{


}
//--------------------------------------------------------------------------------
float4 PSMAIN(in PSInput input): SV_Target
{
	float4 LightAmbient = float4(0.2f,0.2f,0.2f,1.0f);
	float4 LightDiffuse = float4(1.0f,1.0f,1.0f,1.0f);
	float4 LightSpecular = float4(1.0f,1.0f,1.0f,1.0f);

	float4 MaterialAmbient = float4(1.0f,0.0f,1.0f,1.0f);
	float4 MaterialDiffuse = float4(1.0f,0.8f,0.0f,1.0f);
	float4 MaterialSpecular = float4(1.0f,0.8f,0.0f,1.0f);

	float3 light = input.LightDirWS;
	float lightDistance = length(light);
	light /= lightDistance;

	float3 view = normalize(input.ViewDirWS);
		float3 normal = normalize(input.NormalWS);
		float3 reflection = reflect(-light,normal);

		float4 ambientIntensity = LightAmbient*MaterialAmbient;
		float lightAttenuation = 1.0f;

	float diffuseFactor = max(dot(light,normal),0.0f);
	float4 diffuse = diffuseFactor*(LightDiffuse*MaterialDiffuse);


		float specularFactor = pow(max(dot(reflection,view),0.0f),50.0f);
	float4 specularIntensity  = lightAttenuation * specularFactor * (LightSpecular * MaterialSpecular);
		if(dot(light,normal) < 0.0f)
			specularIntensity	  = float4(0.0f,0.0f,0.0f,1.0f);

	float fDistanceSquared = dot(input.LightDirWS.xyz,input.LightDirWS.xyz);
	float fDepth = fDistanceSquared*DepthBias;

	float shadowFactor		  = ShadowMap.SampleCmp(ShadowMapSampler,-light,lightDistance/LightRange.x - DepthBias);
	return (ambientIntensity + (diffuse+specularIntensity)*shadowFactor);
	/*return float4(fDepth,fDepth,fDepth,fDepth);*/
}