//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//OmniDÝrectionalDepthVS.h
//--------------------------------------------------------------------------------
cbuffer TransformFinalVS
{
	matrix gWorld;
	matrix gView;
	matrix WorldViewProjMatrix;	
	matrix WorldInverseTranspose;
};
//--------------------------------------------------------------------------------
cbuffer LightConstantsFinal
{
	float4 LightPosWS;
	float4 ViewPosWS;
};
//--------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 PositionOS : POSITION;
	float4 NormalOS	  : NORMAL;
};
//--------------------------------------------------------------------------------
struct VS_OUTPUT
{	
	float4 PositionCS : SV_Position;
	float3 NormalWS : Attrib0;
	float3 LightDirWS : Attrib1;
	float3 ViewDirWS :  Attrib2;
};
//--------------------------------------------------------------------------------
VS_OUTPUT VSMAIN( in VS_INPUT input )
{
	VS_OUTPUT output;
	float3 posWS = mul(gWorld, float4(input.PositionOS.xyz,1.0f)).xyz;
	output.PositionCS = mul(input.PositionOS,WorldViewProjMatrix);
	output.NormalWS = mul(float4(input.NormalOS.xyz,0.0f),WorldInverseTranspose);
	output.LightDirWS = LightPosWS.xyz - posWS;
	output.ViewDirWS= ViewPosWS.xyz - posWS;
	return output;
}