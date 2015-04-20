//--------------------------------------------------------------------------------
// DepthPS.hlsl
//
// This pixel shader outputs the linear view space depth to the render target.
// The linear space is used to avoid precision issues on the later calculations.
//
// Copyright (C) 2009 Jason Zink.  All rights reserved.
//--------------------------------------------------------------------------------


struct VS_OUTPUT
{
	float4 position : SV_Position;
	
};

float4 PSMAIN( in VS_OUTPUT input ) : SV_Target
{
	float depth = float4(input.position.z,input.position.z,input.position.z,input.position.z);
	return depth;
}

