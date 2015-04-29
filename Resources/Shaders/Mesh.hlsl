//======================================================================
//
//	Bokeh Sample
//  by MJP
//  http://mynameismjp.wordpress.com/
//
//  All code and content licensed under Microsoft Public License (Ms-PL)
//
//======================================================================

//======================================================================================
// Constants
//======================================================================================
static const uint NumCascades = 4;

//======================================================================================
// Constant buffers
//======================================================================================
cbuffer VSConstants : register(cb0)
{
    float4x4 gWorld : packoffset(c0);
	float4x4 gView : packoffset(c4);
    float4x4 WorldViewProjMatrix : packoffset(c8);
}

cbuffer PSConstants : register(cb0)
{
    float3 LightDirWS : packoffset(c0);
    float3 LightColor : packoffset(c1);
    float3 CameraPosWS : packoffset(c2);
	float4x4 ShadowMatrices[4] : packoffset(c3);
	float4 CascadeSplits : packoffset(c19);
}

//======================================================================================
// Samplers
//======================================================================================
Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D ShadowMap : register(t2);
SamplerState AnisoSampler : register(s0);
SamplerComparisonState ShadowSampler : register(s1);
//======================================================================================
// Input/Output structs
//======================================================================================
struct VSInput
{
    float4 PositionOS 		: POSITION;
    float3 NormalOS 		: NORMAL;
    float2 TexCoord 		: TEXCOORDS0;
	float3 TangentOS 		: TANGENT;
	float3 BinormalOS		: BINORMAL;
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

float SampleShadowCascade(in float3 positionWS, in uint cascadeIdx)
{
	float4x4 shadowMatrix = ShadowMatrices[cascadeIdx];
	float3 shadowPosition = mul(float4(positionWS, 1.0f), shadowMatrix).xyz;
	float2 shadowTexCoord = shadowPosition.xy;
	float shadowDepth = shadowPosition.z;

	// Edge tap smoothing
	const int Radius = 2;
	const float ShadowMapSize = 2048.0f * 2;
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
float3 ShadowVisibility(in float3 positionWS, in float depthVS)
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

	shadowVisibility = SampleShadowCascade(positionWS, cascadeIdx);

	// Sample the next cascade, and blend between the two results to
	// smooth the transition
	const float BlendThreshold = 0.1f;
	float nextSplit = CascadeSplits[cascadeIdx];
	float splitSize = i == 0 ? nextSplit : nextSplit - CascadeSplits[cascadeIdx - 1];
	float splitDist = (nextSplit - depthVS) / splitSize;

	float nextSplitVisibility = SampleShadowCascade(positionWS, cascadeIdx + 1);
	float lerpAmt = smoothstep(0.0f, BlendThreshold, splitDist);
	shadowVisibility = lerp(nextSplitVisibility, shadowVisibility, lerpAmt);

	return shadowVisibility;
}

//--------------------------------------------------------------------------------------
// Evalutes the H-Basis coefficients in the tangent space normal direction
//--------------------------------------------------------------------------------------
float3 GetHBasisIrradiance(in float3 n, in float3 H0, in float3 H1, in float3 H2, in float3 H3)
{
	float3 color = 0.0f;

    // Band 0
    color += H0 * (1.0f / sqrt(2.0f * 3.14159f));

    // Band 1
    color += H1 * -sqrt(1.5f / 3.14159f) * n.y;
    color += H2 * sqrt(1.5f / 3.14159f) * (2 * n.z - 1.0f);
    color += H3 * -sqrt(1.5f / 3.14159f) * n.x;

	return color;
}

//======================================================================================
// Pixel Shader
//======================================================================================
float4 PS(in PSInput input) : SV_Target
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

	//// Sample baked diffuse irradiance from the H-basis coefficients
	//float3 bakedLight = GetHBasisIrradiance(normalTS, input.H0, input.H1, input.H2, input.H3) / 3.14159f;
	//diffuse += bakedLight * diffuseAlbedo;

	float3 shadowVisibility = ShadowVisibility(input.PositionWS, input.DepthVS);

	// Add in the primary directional light
	diffuse += saturate(dot(normalWS, LightDirWS)) * LightColor * shadowVisibility * (1.0f / 3.14159f);

	// Calculate the specular lighting analytically
	static const float SpecularPower = 64.0f;
	float3 L = LightDirWS;
	float3 V = normalize(CameraPosWS - input.PositionWS);
	float3 H = normalize(V + L);
	float NdotH = saturate(dot(normalWS, H));
	specular += pow(NdotH, SpecularPower) * ((SpecularPower + 8.0f) / (8.0f * 3.14159265f)) * LightColor * shadowVisibility;

	float3 output = diffuse * diffuseAlbedo + specular * specularAlbedo;

    return float4(max(output, 0.0001f), 1.0f);
}