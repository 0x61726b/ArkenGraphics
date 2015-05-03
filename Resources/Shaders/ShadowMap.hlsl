//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static const uint NumCascades = 4;
//-----------------------------------------------------------------------------
cbuffer VSConstants : register(cb0)
{
	float4x4 gWorld : packoffset(c0);
	float4x4 gView : packoffset(c4);
	float4x4 WorldViewProjMatrix : packoffset(c8);
	float4x4 LightView : packoffset(c12);
	float4x4 LightProj : packoffset(c16);
};
//-----------------------------------------------------------------------------
cbuffer PSConstants : register(cb0)
{
	float3 LightDirWS; //12
	float3 LightColor; //24
	float3 CameraPosWS; //36

};
//-----------------------------------------------------------------------------
Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D ShadowMap : register(t2);
SamplerState AnisoSampler : register(s0);
SamplerComparisonState ShadowSampler : register(s1);
//-----------------------------------------------------------------------------
struct VSInput
{
	float4 PositionOS 		: POSITION;
	float3 NormalOS 		: NORMAL;
	float2 TexCoord 		: TEXCOORDS0;
	float4 TangentOS 		: TANGENT;
};

struct VSOutput
{
	float4 PositionCS 		: SV_Position;
	float3 PositionWS 		: POSITIONWS;
	float3 NormalWS 		: NORMALWS;
	float3 TangentWS 		: TANGENTWS;
	float3 BinormalWS 		: BINORMALWS;
	float4 LightPosWS		: TEXCOORDS0;

	float2 TexCoord 		: TEXCOORDS1;
	float DepthVS			: DEPTHVS;
};

struct PSInput
{
	float4 PositionSS 		: SV_Position;
	float3 PositionWS 		: POSITIONWS;
	float3 NormalWS 		: NORMALWS;
	float3 TangentWS 		: TANGENTWS;
	float3 BinormalWS 		: BINORMALWS;
	float4 LightPosWS		: TEXCOORDS0;

	float2 TexCoord 		: TEXCOORDS1;
	float DepthVS			: DEPTHVS;
};
static const float4 vCascadeColorsMultiplier[8] =
{
	float4 (1.5f,0.0f,0.0f,1.0f),
	float4 (0.0f,1.5f,0.0f,1.0f),
	float4 (0.0f,0.0f,5.5f,1.0f),
	float4 (1.5f,0.0f,5.5f,1.0f),
	float4 (1.5f,1.5f,0.0f,1.0f),
	float4 (1.0f,1.0f,1.0f,1.0f),
	float4 (0.0f,1.0f,5.5f,1.0f),
	float4 (0.5f,3.5f,0.75f,1.0f)
};
//-----------------------------------------------------------------------------
VSOutput VSMain(in VSInput input,in uint VertexID : SV_VertexID)
{
	VSOutput output;

	output.PositionWS = mul((input.PositionOS),gWorld).xyz;

	output.DepthVS = mul(float4(output.PositionWS,1.0f),gView).z;

	output.PositionCS = mul((input.PositionOS),WorldViewProjMatrix);

	output.NormalWS = normalize(mul(input.NormalOS,(float3x3)gWorld));

	output.TangentWS = normalize(mul(input.TangentOS.xyz,(float3x3)gWorld));

	float3 bitangentWS = normalize(cross(output.NormalWS,output.TangentWS)) * input.TangentOS.w;

	output.BinormalWS = bitangentWS;

	output.TexCoord = input.TexCoord;

	output.LightPosWS = mul( input.PositionOS , mul(LightView,LightProj) );

	return output;
}

//--------------------------------------------------------------------------------------
// Computes the visibility term by performing the shadow test
//--------------------------------------------------------------------------------------
float4 ShadowVisibility(in float4 positionWS,in float depthVS)
{
	positionWS.xyz /= positionWS.w;

	if( positionWS.x < -1.0f || positionWS.x > 1.0f || 
	   positionWS.y < -1.0f || positionWS.y > 1.0f ||
	    positionWS.z < 0.0f  || positionWS.z > 1.0f ) return float4(0,0,0,0);

	positionWS.x = positionWS.x*0.5f + 0.5f;
	positionWS.y = positionWS.y*(-0.5f) + 0.5f;

	positionWS.z -= 0.01f;

	float shadowMapDepth = ShadowMap.Sample(AnisoSampler,positionWS.xy).r;

	float shadowFactor = positionWS.z <= shadowMapDepth;

	return shadowFactor;
}


float4 PSMain(in PSInput input): SV_Target
{
	   // Normalize after interpolation
    float3 normalWS = normalize(input.NormalWS);
	float3 normalTS = float3(0, 0, 1);

	float3 tangentWS = normalize(input.TangentWS);
	float3 binormalWS = normalize(input.BinormalWS);
	float3x3 tangentToWorld = float3x3(tangentWS, binormalWS, normalWS);

	// Sample the normal map, and convert the normal to world space
	normalTS.xyz = NormalMap.Sample(AnisoSampler, input.TexCoord).xyz * 2.0f - 1.0f;
	normalTS.z = sqrt(1.0f - ((normalTS.x * normalTS.x) + (normalTS.y * normalTS.y)));
	normalWS = normalize(mul(normalTS, tangentToWorld));

	float4 texSample = DiffuseMap.Sample(AnisoSampler, input.TexCoord);
	float3 diffuseAlbedo = texSample.xyz;
	float specularAlbedo = texSample.w;

	float3 diffuse = 0.0f;
	float3 specular = 0.0f;

	float4 ambient = float4(0.1f,0.1f,0.1f,1.0f);

	input.LightPosWS.xyz /= input.LightPosWS.w;

	//if( input.LightPosWS.x < -1.0f || input.LightPosWS.x > 1.0f || 
	//   input.LightPosWS.y < -1.0f || input.LightPosWS.y > 1.0f ||
	//    input.LightPosWS.z < 0.0f  || input.LightPosWS.z > 1.0f ) return ambient;

	input.LightPosWS.x = input.LightPosWS.x*0.5f + 0.5f;
	input.LightPosWS.y = input.LightPosWS.y*(-0.5f) + 0.5f;

	input.LightPosWS.z -= 0.01f;

	float shadowMapDepth = ShadowMap.SampleCmpLevelZero(ShadowSampler,input.LightPosWS.xy,input.LightPosWS.z);

	float shadowFactor = input.LightPosWS.z <= shadowMapDepth;

	float shadowVisibility = shadowMapDepth;
	
	// Add in the primary directional light
	diffuse += saturate(dot(normalWS, LightDirWS)) * LightColor * shadowVisibility * (1.0f / 3.14159f);

	// Calculate the specular lighting analytically
	static const float SpecularPower = 64.0f;
	float3 L = LightDirWS;
	float3 V = normalize(CameraPosWS - input.PositionWS);
	float3 H = normalize(V + L);
	float NdotH = saturate(dot(normalWS, H));
	specular += pow(NdotH, SpecularPower) * ((SpecularPower + 8.0f) / (8.0f * 3.14159265f)) * LightColor * shadowVisibility;

	

	float3 output = (diffuse  + ambient.xyz)* diffuseAlbedo + specular * specularAlbedo;
	float4 finalColor = float4(max(output,0.0001f),1.0f);
	finalColor.a = texSample.a;
	uint cascadeIdx = 0;


	return float4(output,1.0f);
	

}