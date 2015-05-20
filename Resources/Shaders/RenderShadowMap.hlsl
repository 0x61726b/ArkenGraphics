////////////////////////////////////////////////////////////////////////////////
// Filename: shadow.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer RenderShadowTransform
{
	matrix gWorld;
	matrix gView;
	matrix gProj;
	matrix ViewProjMatrix;
	matrix WorldViewProjection;
	matrix WorldViewMatrix;
};
////////////////////////////////////////////////////////////////////////////////
struct VertexInputType
{
	float4 position : POSITION;
};
////////////////////////////////////////////////////////////////////////////////
struct PixelInputType
{
	float4 position : SV_POSITION;

};
////////////////////////////////////////////////////////////////////////////////
cbuffer LightTransform
{
	float4 lightPosition;
};
Texture2D DepthNormalBuffer : register(t0);
SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap  : register(s1);

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType VSMAIN(VertexInputType input)
{
	PixelInputType output;
	output.position = mul(input.position,ViewProjMatrix);

	return output;
}
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PSMAIN(PixelInputType input): SV_TARGET
{
	float4 ambient = float4(0.0f,0.0f,0.0f,0.0f);
	float4 diffuse = float4(0.0f,0.0f,0.0f,0.0f);
	float4 spec    = float4(0.0f,0.0f,0.0f,0.0f);

	float depth = DepthNormalBuffer.Sample(SampleTypeWrap,input.position);
	depth = 1.0f - (1.0 - depth) * 25.0f;
	return depth;

}