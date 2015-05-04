//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//OmniDÝrectionalDepthVS.h
//--------------------------------------------------------------------------------
cbuffer TransformPS
{
	matrix gWorld;
	matrix gView;
	matrix WorldViewProjMatrix;	
};
//--------------------------------------------------------------------------------
cbuffer LightConstantsPS
{
	float4 LightRange;
};
//--------------------------------------------------------------------------------
struct PSInput
{	
	float4 PositionCS : SV_Position;
	uint	shadowMapIndex				: SV_RenderTargetArrayIndex;
	float3 LightDirWS : Attrib0;
};
//--------------------------------------------------------------------------------
float PSMAIN( in PSInput v ) : SV_Depth
{
	float depth = length(v.LightDirWS) / LightRange.x;
	return depth;
}