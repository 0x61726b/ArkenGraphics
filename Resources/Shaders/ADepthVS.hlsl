//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PerspectiveView.h
//--------------------------------------------------------------------------------

cbuffer TransformsDepth
{
	matrix gWorld;
	matrix gView;
	matrix WorldViewProjMatrix;	
};


struct VS_INPUT
{
	float4 PositionOS : POSITION;
};

struct VS_OUTPUT
{
	float4 PositionCS : SV_Position;
};


VS_OUTPUT VSMAIN( in VS_INPUT v )
{
	VS_OUTPUT output;
	output.PositionCS = mul( ( v.PositionOS), WorldViewProjMatrix );
	return output;
}

float4 PSMAIN() : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}