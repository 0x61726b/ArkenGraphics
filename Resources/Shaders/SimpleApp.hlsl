//-----------------------------------------------------------------------------
cbuffer Transforms
 {
	float4x4 WorldViewProjMatrix;
};

//-----------------------------------------------------------------------------
struct VS_INPUT
{
	float4 position : POSITION;
	/*float4 Color    : COLOR;*/
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	/*float4 Color	: COLOR;*/
};

//-----------------------------------------------------------------------------
VS_OUTPUT VSMain( in VS_INPUT v )
{
	VS_OUTPUT o = (VS_OUTPUT)0;

	o.position = mul(v.position,WorldViewProjMatrix);
	/*o.Color = v.Color;*/

	return o;
}


//-----------------------------------------------------------------------------
float4 PSMain( in VS_OUTPUT input ) : SV_Target
{

	return( float4(1,1,1,1) );
}
//-----------------------------------------------------------------------------