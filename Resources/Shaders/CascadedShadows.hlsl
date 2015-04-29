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
};
//-----------------------------------------------------------------------------
cbuffer PSConstants : register(cb0)
{
	float3 LightDirWS; //12
	float3 LightColor; //24
	float3 CameraPosWS; //36
	float4x4 ShadowMatrix; //100
	float4x4 ShadowMatrix2; //164
	float4x4 ShadowMatrix3; //228
	float4x4 ShadowMatrix4; //292
	float4 CascadeSplits; //308
	float4x4 ShadowMatrices[4];
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

	float2 TexCoord 		: TEXCOORDS;
	float DepthVS			: DEPTHVS;
};

struct PSInput
{
	float4 PositionSS 		: SV_Position;
	float3 PositionWS 		: POSITIONWS;
	float3 NormalWS 		: NORMALWS;
	float3 TangentWS 		: TANGENTWS;
	float3 BinormalWS 		: BINORMALWS;

	float2 TexCoord 		: TEXCOORDS;
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

	return output;
}
float SampleShadowCascade(in float3 positionWS,in uint cascadeIdx)
{
	float4x4 shadowMatrix = ShadowMatrices[cascadeIdx];

	float3 shadowPosition = mul(float4(positionWS,1.0f),shadowMatrix).xyz;
	float2 shadowTexCoord = shadowPosition.xy;
	float shadowDepth = shadowPosition.z;

	const int Radius = 2;
	const float ShadowMapSize = 2048*2;
	const int NumSamples = (Radius * 2 + 1) * (Radius * 2 + 1);

	float2 fracs = frac(shadowTexCoord.xy * ShadowMapSize);
	float leftEdge = 1.0f - fracs.x;
	float rightEdge = fracs.x;
	float topEdge = 1.0f - fracs.y;
	float bottomEdge = fracs.y;

	float shadowVisibility = 0.0f;

	[unroll(NumSamples)]
	for (int y = -Radius; y <= Radius; y++)
	{
		[unroll(NumSamples)]
		for (int x = -Radius; x <= Radius; x++)
		{
			float2 offset = float2(x, y) * (1.0f / ShadowMapSize);
			float2 sampleCoord = shadowTexCoord + offset;
			float sample = ShadowMap.SampleCmp(ShadowSampler, sampleCoord, shadowDepth).x;

			float xWeight = 1;
			float yWeight = 1;

			if(x == -Radius)
				xWeight = leftEdge;
			else if(x == Radius)
				xWeight = rightEdge;

			if(y == -Radius)
				yWeight = topEdge;
			else if(y == Radius)
				yWeight = bottomEdge;

			shadowVisibility += sample * xWeight * yWeight;
		}
	}
	shadowVisibility  /= NumSamples;
	shadowVisibility *= 1.5f;
	return shadowVisibility;
}
//--------------------------------------------------------------------------------------
// Computes the visibility term by performing the shadow test
//--------------------------------------------------------------------------------------
float3 ShadowVisibility(in float3 positionWS,in float depthVS)
{
	float3 shadowVisibility = 1.0f;
	uint cascadeIdx = 0;

	// Figure out which cascade to sample from
	[unroll]
	for(uint i = 0; i < NumCascades - 1; ++i)
	{
		[flatten]
		if(depthVS > CascadeSplits[i])
			cascadeIdx = i + 1;
	}

	shadowVisibility = SampleShadowCascade(positionWS,cascadeIdx);

	//// Sample the next cascade, and blend between the two results to
	//// smooth the transition
	const float BlendThreshold = 0.1f;
	float nextSplit = CascadeSplits[cascadeIdx];
	float splitSize = i == 0 ? nextSplit : nextSplit - CascadeSplits[cascadeIdx - 1];
	float splitDist = (nextSplit - depthVS) / splitSize;

	float nextSplitVisibility = SampleShadowCascade(positionWS,cascadeIdx + 1);
	float lerpAmt = smoothstep(0.0f,BlendThreshold,splitDist);
	shadowVisibility = lerp(nextSplitVisibility,shadowVisibility,lerpAmt);

	return shadowVisibility;
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

	float3 shadowVisibility = ShadowVisibility(input.PositionWS,input.DepthVS);

	// Add in the primary directional light
	diffuse += saturate(dot(normalWS, LightDirWS)) * LightColor * shadowVisibility * (1.0f / 3.14159f);

	// Calculate the specular lighting analytically
	static const float SpecularPower = 64.0f;
	float3 L = LightDirWS;
	float3 V = normalize(CameraPosWS - input.PositionWS);
	float3 H = normalize(V + L);
	float NdotH = saturate(dot(normalWS, H));
	specular += pow(NdotH, SpecularPower) * ((SpecularPower + 8.0f) / (8.0f * 3.14159265f)) * LightColor * shadowVisibility;

	float3 ambient = float3(0.1f,0.1f,0.1f);

	float3 output = (diffuse  + ambient)* diffuseAlbedo + specular * specularAlbedo;
	float4 finalColor = float4(max(output,0.0001f),1.0f);
	finalColor.a = texSample.a;
	uint cascadeIdx = 0;

	[unroll]
	for(uint i = 0; i < NumCascades - 1; ++i)
	{
		[flatten]
		if(input.DepthVS > CascadeSplits[i])
			cascadeIdx = i + 1;
	}
	return float4(max(output,0.0001f),1.0f);

}