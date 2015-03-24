//-----------------------------------------------------------------------------
cbuffer Transforms
 {
	matrix gWorld;
	matrix gView;
	matrix gProj;
};

//-----------------------------------------------------------------------------
struct VS_INPUT
{
	float4 position : SV_Position;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float4 color : COLOR;
};

//-----------------------------------------------------------------------------
VS_OUTPUT VSMain( in VS_INPUT v )
{
	VS_OUTPUT o = (VS_OUTPUT)0;

	o.position = mul(v.position,gWorld);
	o.position = mul(o.position,gView);
	o.position = mul(o.position,gProj);

	o.color = v.color;

	return o;
}


//-----------------------------------------------------------------------------
float4 PSMain( in VS_OUTPUT input ) : SV_Target
{
	float4 color = input.color;

	return( color );
}
//-----------------------------------------------------------------------------