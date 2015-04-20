//--------------------------------------------------------------------------------
// DepthVS
//
// This vertex shader outputs the linear view space depth to the pixel shader.
// The linear space is used to avoid precision issues on the later calculations.
//
// Copyright (C) 2009 Jason Zink.  All rights reserved.
//--------------------------------------------------------------------------------

cbuffer Transforms
{
	matrix WorldViewProjMatrix;	
	matrix WorldViewMatrix;
};


struct VS_INPUT
{
	float4 position : POSITION;
	float3 normal	: NORMAL;
	float2 tex		: TEXCOORDS0;
	
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 tex : TEXCOORDS;
};


VS_OUTPUT VSMAIN( in VS_INPUT v )
{
	VS_OUTPUT output;
	output.position = mul( ( v.position), WorldViewProjMatrix );
	output.tex = v.tex;
	return output;
}

