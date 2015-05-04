//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//OmniDÝrectionalDepthVS.h
//--------------------------------------------------------------------------------
static const uint ShadowMapCount = 6;

struct GSInput
{
	float4	PositionWS			: SV_Position;
	float3  LightDirWS			: Attrib0;
};

struct GSOutput
{
	float4	PositionCS					: SV_Position;
	uint	shadowMapIndex				: SV_RenderTargetArrayIndex;
	float3  LightDirWS					: Attrib0;
};

cbuffer cbPerFrame				: register(b0)
{
	matrix LightViewProjMatrices[ShadowMapCount];
};

[instance(ShadowMapCount)]
[maxvertexcount(3)]
void GSMAIN(uint shadowMapId : SV_GSInstanceID, triangle GSInput input[3], inout TriangleStream<GSOutput> outputStream)
{
	GSOutput output;
	output.shadowMapIndex = shadowMapId;
	
	for (uint vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
	{
		output.LightDirWS = input[vertexIndex].LightDirWS;
		output.PositionCS = mul(input[vertexIndex].PositionWS,LightViewProjMatrices[shadowMapId]);
		
		outputStream.Append(output);
	}
	
	outputStream.RestartStrip();
}