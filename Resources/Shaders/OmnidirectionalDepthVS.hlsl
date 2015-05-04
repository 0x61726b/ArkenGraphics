//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//OmniDÝrectionalDepthVS.h
//--------------------------------------------------------------------------------
cbuffer TransformsDepth
{
	matrix gWorld;
	matrix gView;
	matrix WorldViewProjMatrix;	
};
//--------------------------------------------------------------------------------
cbuffer LightConstants
{
	float4 LightPosWS;
};
//--------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 PositionOS : POSITION;
};
//--------------------------------------------------------------------------------
struct VS_OUTPUT
{	
	float4 PositionWS : SV_Position;
	float3 LightDirWS : Attrib0;
};
//--------------------------------------------------------------------------------
VS_OUTPUT VSMAIN( in VS_INPUT v )
{
	VS_OUTPUT output;
	output.PositionWS = mul(float4(v.PositionOS.xyz,1.0f),gWorld);
	output.LightDirWS = output.PositionWS.xyz - LightPosWS.xyz;
	return output;
}