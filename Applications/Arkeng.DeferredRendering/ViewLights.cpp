//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "ViewLights.h"
#include "ArkEntity3D.h"
#include "Scene.h"
#include "Dx11Texture2dConfig.h"
#include "ArkLog.h"
#include "IParameterManager.h"
#include "PipelineManager.h"
#include "Dx11Texture2d.h"
#include "Dx11DepthStencilStateConfig.h"
//#include "ViewGBuffer.h"
#include "ArkGeometryGenerator11.h"
#include "Dx11BlendStateConfig.h"
#include "AppSettings.h"
#include "Dx11RasterizerStateConfig.h"
#include "Dx11SamplerStateConfig.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
static float Clamp(float x,float low,float high)
{
	if(x < low)
		x = low;
	if(x > high)
		x = high;
	return x;
}
//--------------------------------------------------------------------------------
ViewLights::ViewLights(ArkRenderer11& Renderer)
{
	ViewMatrix =  XMMatrixIdentity() ;
	ProjMatrix =  XMMatrixIdentity() ;

	Dx11SamplerStateConfig SamplerConfig;
	SamplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	int iShadowSamplerState = Renderer.CreateSamplerState(&SamplerConfig);
	auto samplerParam = Renderer.m_pParamMgr->GetSamplerStateParameterRef( std::wstring( L"ShadowMapSampler" ) );
	samplerParam->InitializeParameterData( &iShadowSamplerState );
	// Create a depth stencil state with no depth testing, and with stencil testing
	// enabled to make sure we only light pixels where we rendered to the G-Buffer
	Dx11DepthStencilStateConfig dsConfig;
	dsConfig.DepthEnable = FALSE;
	dsConfig.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsConfig.DepthFunc = D3D11_COMPARISON_LESS;
	dsConfig.StencilEnable = true;
	dsConfig.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsConfig.StencilWriteMask = 0;
	dsConfig.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsConfig.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsConfig.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsConfig.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	dsConfig.BackFace = dsConfig.FrontFace;

	m_iDisabledDSState = Renderer.CreateDepthStencilState(&dsConfig);

	if(m_iDisabledDSState == -1)
		ArkLog::Get(LogType::Renderer).Output(L"Failed to create light depth stencil state");

	// Create a depth stencil state with less-than-equal depth testing
	dsConfig.DepthEnable = TRUE;
	dsConfig.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	m_iLessThanDSState = Renderer.CreateDepthStencilState(&dsConfig);

	if(m_iLessThanDSState == -1)
		ArkLog::Get(LogType::Renderer).Output(L"Failed to create light depth stencil state");

	// Create a depth stencil state with greater-than-equal depth testing
	dsConfig.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	m_iGreaterThanDSState = Renderer.CreateDepthStencilState(&dsConfig);

	if(m_iGreaterThanDSState == -1)
		ArkLog::Get(LogType::Renderer).Output(L"Failed to create light depth stencil state");

	// Create a blend state for additive blending
	Dx11BlendStateConfig blendConfig;
	blendConfig.AlphaToCoverageEnable = false;
	blendConfig.IndependentBlendEnable = false;
	for(int i = 0; i < 8; ++i)
	{
		blendConfig.RenderTarget[i].BlendEnable = true;
		blendConfig.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendConfig.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
		blendConfig.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
		blendConfig.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendConfig.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendConfig.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
		blendConfig.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	m_iBlendState = Renderer.CreateBlendState(&blendConfig);

	if(m_iBlendState == -1)
		ArkLog::Get(LogType::Renderer).Output(L"Failed to create light blend state");

	// Create a rasterizer state with scissoring enabled
	Dx11RasterizerStateConfig rsConfig;
	rsConfig.MultisampleEnable = TRUE;
	rsConfig.ScissorEnable = TRUE;
	m_iScissorRSState = Renderer.CreateRasterizerState(&rsConfig);

	// Create a rasterizer state with back-face culling enabled
	rsConfig.ScissorEnable = FALSE;
	rsConfig.CullMode = D3D11_CULL_BACK;
	m_iBackFaceCullRSState = Renderer.CreateRasterizerState(&rsConfig);

	// Create a rasterizer state with front-face culling enabled
	rsConfig.CullMode = D3D11_CULL_FRONT;
	m_iFrontFaceCullRSState = Renderer.CreateRasterizerState(&rsConfig);

	if(m_iScissorRSState == -1 || m_iBackFaceCullRSState == -1 || m_iFrontFaceCullRSState == -1)
		ArkLog::Get(LogType::Renderer).Output(L"Failed to create light rasterizer state");

	// Generate geometry for a full screen quad
	m_QuadGeometry = GeometryPtr(new ArkGeometry11());
	ArkGeometryGenerator11::GenerateFullScreenQuad(m_QuadGeometry);
	m_QuadGeometry->LoadToBuffers();

	// Generate geometry for a sphere
	m_SphereGeometry = GeometryPtr(new ArkGeometry11());
	ArkGeometryGenerator11::GenerateSphere(m_SphereGeometry,8,7,1.0f);
	m_SphereGeometry->LoadToBuffers();

	// Generate geometry for a cone
	m_ConeGeometry = GeometryPtr(new ArkGeometry11());
	ArkGeometryGenerator11::GenerateCone(m_ConeGeometry,8,2,1.0f,1.0f);
	m_ConeGeometry->LoadToBuffers();

	for(int gBufferOptMode = 0; gBufferOptMode < GBufferOptMode::NumSettings; ++gBufferOptMode)
	{
		for(int lightOptMode = 0; lightOptMode < LightOptMode::NumSettings; ++lightOptMode)
		{
			for(int aaMode = 0; aaMode < AAMode::NumSettings; ++aaMode)
			{

				// We'll create permutations of our shaders bases on optimization mods
				D3D_SHADER_MACRO defines[7];
				defines[0].Name = "POINTLIGHT";
				defines[0].Definition = "1";
				defines[1].Name = "SPOTLIGHT";
				defines[1].Definition = "0";
				defines[2].Name = "DIRECTIONALLIGHT";
				defines[2].Definition = "0";
				defines[3].Name = "GBUFFEROPTIMIZATIONS";
				defines[3].Definition = gBufferOptMode == GBufferOptMode::OptEnabled ? "1" : "0";
				defines[4].Name = "LIGHTVOLUMES";
				defines[4].Definition = lightOptMode == LightOptMode::Volumes ? "1" : "0";
				defines[5].Name = "MSAA";
				defines[5].Definition = aaMode == AAMode::MSAA ? "1" : "0";
				defines[6].Name = NULL;
				defines[6].Definition = NULL;

				// Point light shaders
				m_PointLightEffect[gBufferOptMode][lightOptMode][aaMode].SetVertexShader(
					Renderer.LoadShader(VERTEX_SHADER,
					std::wstring(L"Lights.hlsl"),
					std::wstring(L"VSMain"),
					std::wstring(L"vs_5_0"),
					defines));
				_ASSERT(m_PointLightEffect[gBufferOptMode][lightOptMode][aaMode].GetVertexShader() != -1);

				m_PointLightEffect[gBufferOptMode][lightOptMode][aaMode].SetPixelShader(
					Renderer.LoadShader(PIXEL_SHADER,
					std::wstring(L"Lights.hlsl"),
					std::wstring(L"PSMain"),
					std::wstring(L"ps_5_0"),
					defines));
				_ASSERT(m_PointLightEffect[gBufferOptMode][lightOptMode][aaMode].GetPixelShader() != -1);

				m_PointLightEffect[gBufferOptMode][lightOptMode][aaMode].m_iBlendState = m_iBlendState;
				m_PointLightEffect[gBufferOptMode][lightOptMode][aaMode].m_iDepthStencilState = m_iDisabledDSState;
				m_PointLightEffect[gBufferOptMode][lightOptMode][aaMode].m_iRasterizerState = m_iBackFaceCullRSState;
				m_PointLightEffect[gBufferOptMode][lightOptMode][aaMode].m_uStencilRef = 1;

				// Spot light shaders
				defines[0].Definition = "0";
				defines[1].Definition = "1";

				m_SpotLightEffect[gBufferOptMode][lightOptMode][aaMode].SetVertexShader(
					Renderer.LoadShader(VERTEX_SHADER,
					std::wstring(L"Lights.hlsl"),
					std::wstring(L"VSMain"),
					std::wstring(L"vs_5_0"),
					defines));
				_ASSERT(m_SpotLightEffect[gBufferOptMode][lightOptMode][aaMode].GetVertexShader() != -1);

				m_SpotLightEffect[gBufferOptMode][lightOptMode][aaMode].SetPixelShader(
					Renderer.LoadShader(PIXEL_SHADER,
					std::wstring(L"Lights.hlsl"),
					std::wstring(L"PSMain"),
					std::wstring(L"ps_5_0"),
					defines));
				_ASSERT(m_SpotLightEffect[gBufferOptMode][lightOptMode][aaMode].GetPixelShader() != -1);

				m_SpotLightEffect[gBufferOptMode][lightOptMode][aaMode].m_iBlendState = m_iBlendState;
				m_SpotLightEffect[gBufferOptMode][lightOptMode][aaMode].m_iDepthStencilState = m_iDisabledDSState;
				m_SpotLightEffect[gBufferOptMode][lightOptMode][aaMode].m_iRasterizerState = m_iBackFaceCullRSState;
				m_SpotLightEffect[gBufferOptMode][lightOptMode][aaMode].m_uStencilRef = 1;

				// Directional light shaders
				defines[1].Definition = "0";
				defines[2].Definition = "1";

				m_DirectionalLightEffect[gBufferOptMode][lightOptMode][aaMode].SetVertexShader(
					Renderer.LoadShader(VERTEX_SHADER,
					std::wstring(L"Lights.hlsl"),
					std::wstring(L"VSMain"),
					std::wstring(L"vs_5_0"),
					defines));
				_ASSERT(m_DirectionalLightEffect[gBufferOptMode][lightOptMode][aaMode].GetVertexShader() != -1);

				m_DirectionalLightEffect[gBufferOptMode][lightOptMode][aaMode].SetPixelShader(
					Renderer.LoadShader(PIXEL_SHADER,
					std::wstring(L"Lights.hlsl"),
					std::wstring(L"PSMain"),
					std::wstring(L"ps_5_0"),
					defines));
				_ASSERT(m_DirectionalLightEffect[gBufferOptMode][lightOptMode][aaMode].GetPixelShader() != -1);

				m_DirectionalLightEffect[gBufferOptMode][lightOptMode][aaMode].m_iBlendState = m_iBlendState;
				m_DirectionalLightEffect[gBufferOptMode][lightOptMode][aaMode].m_iDepthStencilState = m_iDisabledDSState;
				m_DirectionalLightEffect[gBufferOptMode][lightOptMode][aaMode].m_iRasterizerState = m_iBackFaceCullRSState;
				m_DirectionalLightEffect[gBufferOptMode][lightOptMode][aaMode].m_uStencilRef = 1;
				
			}
		}
	}

	m_pInvProjMatrix = Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"InvProjMatrix"));
	m_pProjMatrix = Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"ProjMatrix"));
	m_pCameraPos = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"CameraPos"));
	m_pLightViewMatrix = Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"LightView"));
	m_pLightProjMatrix= Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"LightProj"));

	m_pLightPos = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightPos"));
	m_pLightColor = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightColor"));
	m_pLightDirection = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightDirection"));
	m_pLightRange = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightRange"));
	m_pSpotlightAngles = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"SpotlightAngles"));


	m_pGBufferTargets[0] = Renderer.m_pParamMgr->GetShaderResourceParameterRef(std::wstring(L"NormalTexture"));
	m_pGBufferTargets[1] = Renderer.m_pParamMgr->GetShaderResourceParameterRef(std::wstring(L"DiffuseAlbedoTexture"));
	m_pGBufferTargets[2] = Renderer.m_pParamMgr->GetShaderResourceParameterRef(std::wstring(L"SpecularAlbedoTexture"));
	m_pGBufferTargets[3] = Renderer.m_pParamMgr->GetShaderResourceParameterRef(std::wstring(L"PositionTexture"));

	m_pDepthTexture = Renderer.m_pParamMgr->GetShaderResourceParameterRef(std::wstring(L"DepthTexture"));
}
//--------------------------------------------------------------------------------
ViewLights::~ViewLights()
{
}
//--------------------------------------------------------------------------------
void ViewLights::Update(float fTime)
{

}
//--------------------------------------------------------------------------------
void ViewLights::QueuePreTasks(ArkRenderer11* pRenderer)
{
	if(m_pEntity != NULL)
	{
		XMMATRIX view = m_pEntity->Transform.GetView();
		SetViewMatrix(view);
	}

	// Queue this view into the renderer for processing.
	pRenderer->QueueTask(this);
}
//--------------------------------------------------------------------------------
void ViewLights::ExecuteTask(PipelineManager* pPipelineManager,IParameterManager* pParamManager)
{
	// Bind the render target
	pPipelineManager->ClearRenderTargets();
	pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.SetState(0,m_pRenderTarget->m_iResourceRTV);
	pPipelineManager->ApplyRenderTargets();

	// Clear the render target
	/*Vector4f color(0.0f, 0.0f, 0.0f, 0.0f);*/
	pPipelineManager->ClearBuffers(new float[4] { 0,0,0,0 },1.0f,0);

	// Bind the read-only depth buffer
	pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.SetState(m_DepthTarget->m_iResourceDSV);
	pPipelineManager->ApplyRenderTargets();

	// Configure the desired viewports in this pipeline
	ConfigureViewports(pPipelineManager);

	// Set this view's render parameters
	SetRenderParams(pParamManager);

	// Loop through the lights
	for(unsigned int i = 0; i < m_Lights.size(); ++i)
	{
		const LightParams& light = m_Lights[i];

		// Pick the effect based on the shader
		ArkRenderEffect11* pEffect = NULL;
		if(light.Type == Point)
			pEffect = &m_PointLightEffect[GBufferOptMode::Value][LightOptMode::Value][AAMode::Value];
		else if(light.Type == Spot)
			pEffect = &m_SpotLightEffect[GBufferOptMode::Value][LightOptMode::Value][AAMode::Value];
		else if(light.Type == Directional)
			pEffect = &m_DirectionalLightEffect[GBufferOptMode::Value][LightOptMode::Value][AAMode::Value];

		// Set the light params
		XMVECTOR pos = XMVectorSet(light.Position.x,light.Position.y,light.Position.z,1.0f);
		XMVECTOR direction = (XMVector3Normalize(XMLoadFloat3(&light.Direction)));
		if(GBufferOptMode::Enabled())
		{
			// Light position/direction needs to be in view space
			pos = XMVector4Transform(pos,ViewMatrix);
			direction = XMVector4Transform(direction,ViewMatrix);
		}

		pParamManager->SetVectorParameter(m_pLightPos,&pos);
		pParamManager->SetVectorParameter(m_pLightColor,&XMVectorSet(light.Color.x,light.Color.y,light.Color.z,1.0f));
		pParamManager->SetVectorParameter(m_pLightDirection,&direction);
		pParamManager->SetVectorParameter(m_pLightRange,&XMVectorSet(light.Range,1.0f,1.0f,1.0f));
		pParamManager->SetVectorParameter(m_pSpotlightAngles,&XMVectorSet(cosf(light.SpotInnerAngle / 2.0f),
			cosf(light.SpotOuterAngle / 2.0f),
			0.0f,0.0f));

		// Set the rasterizer and depth-stencil state, and draw
		if(LightOptMode::Value == LightOptMode::None)
		{
			pEffect->m_iRasterizerState = m_iBackFaceCullRSState;
			pEffect->m_iDepthStencilState = m_iDisabledDSState;
			pPipelineManager->Draw(*pEffect,m_QuadGeometry,pParamManager);
		}
		else if(LightOptMode::Value == LightOptMode::ScissorRect)
		{
			if(light.Type == Directional)
			{
				// Scissor is pointless for directional lights
				pEffect->m_iRasterizerState = m_iBackFaceCullRSState;
			}
			else
			{
				pEffect->m_iRasterizerState = m_iScissorRSState;



				// Set the scissor rectangle
				D3D11_RECT rect = CalcScissorRect(XMLoadFloat3(&light.Position),light.Range);

				// Set it
				pPipelineManager->RasterizerStage.CurrentState.ScissorRectCount.SetState(1);
				pPipelineManager->RasterizerStage.CurrentState.ScissorRects.SetState(0,rect);
			}

			pEffect->m_iDepthStencilState = m_iDisabledDSState;
			pPipelineManager->Draw(*pEffect,m_QuadGeometry,pParamManager);
		}
		else if(LightOptMode::Value == LightOptMode::Volumes)
		{

			bool intersectsNearPlane = true;
			bool intersectsFarPlane = true;
			if(light.Type != Directional)
			{
				// Try to determine whether the bounding sphere intersects with the clip planes
				XMVECTOR lightPosVS = XMVector3Transform(XMVectorSet(light.Position.x,light.Position.y,light.Position.z,1.0f),ViewMatrix);
				XMFLOAT3 lightPosVS3;
				XMStoreFloat3(&lightPosVS3,lightPosVS);
				intersectsNearPlane = lightPosVS3.z - light.Range <= m_fNearClip;
				intersectsFarPlane = lightPosVS3.z + light.Range >= m_fFarClip;
			}

			if(light.Type == Directional || (intersectsNearPlane && intersectsFarPlane))
			{
				// We still use a quad for directional lights
				pEffect->m_iRasterizerState = m_iBackFaceCullRSState;
				pEffect->m_iDepthStencilState = m_iDisabledDSState;
				pPipelineManager->Draw(*pEffect,m_QuadGeometry,pParamManager);
			}
			else
			{
				// Set an appropriate world matrix for the volume
				XMStoreFloat4x4(&m_WorldMatrix,XMMatrixIdentity());


				if(light.Type == Spot)
				{
					// Determine the scaling factors based on the attenuation
					const float scaleZ = light.Range * 1.1f;
					const float scaleXY = scaleZ * tanf(light.SpotOuterAngle / 2.0f);

					// Set the orientation based on the light direction
					XMVECTOR zAxis = XMVector3Normalize(XMLoadFloat3(&light.Direction));
					XMFLOAT3 zAxis3;
					XMStoreFloat3(&zAxis3,zAxis);

					//Get Perpendicular 
					float xAbs = fabs(zAxis3.x);
					float yAbs = fabs(zAxis3.y);
					float zAbs = fabs(zAxis3.z);
					float minVal = min(min(xAbs,yAbs),zAbs);
					XMVECTOR cross;
					if(xAbs == minVal)
						cross = XMVector3Cross(zAxis,XMVectorSet(1.0f,0.0f,0.0,0.0f));
					else if(yAbs == minVal)
						cross = XMVector3Cross(zAxis,XMVectorSet(0.0f,1.0f,0.0,0.0f));
					else
						cross = XMVector3Cross(zAxis,XMVectorSet(0.0f,0.0f,1.0,0.0f));
					//

					XMVECTOR yAxis = cross;
					XMVECTOR xAxis = XMVector3Cross(yAxis,zAxis);

					//Load to 3
					XMStoreFloat3(&zAxis3,zAxis);
					XMFLOAT3 yAxis3,xAxis3;
					XMStoreFloat3(&xAxis3,xAxis);
					XMStoreFloat3(&yAxis3,yAxis);

					m_WorldMatrix.m[0][0] = xAxis3.x * scaleXY;
					m_WorldMatrix.m[0][1] = xAxis3.y * scaleXY;
					m_WorldMatrix.m[0][2] = xAxis3.z * scaleXY;

					m_WorldMatrix.m[1][0] = yAxis3.x * scaleXY;
					m_WorldMatrix.m[1][1] = yAxis3.y * scaleXY;
					m_WorldMatrix.m[1][2] = yAxis3.z * scaleXY;

					m_WorldMatrix.m[2][2] = zAxis3.x * scaleZ;
					m_WorldMatrix.m[2][2] = zAxis3.y * scaleZ;
					m_WorldMatrix.m[2][2] = zAxis3.z * scaleZ;
				}
				else if(light.Type == Point)
				{
					// Determine the scaling factor based on the attenuation
					const float scaleXYZ = light.Range * 1.1f;

					XMStoreFloat4x4(&m_WorldMatrix,XMMatrixScaling(scaleXYZ,scaleXYZ,scaleXYZ));
				}
				XMMATRIX w = XMLoadFloat4x4(&m_WorldMatrix);
				
				XMStoreFloat4x4(&m_WorldMatrix,w*XMMatrixTranslationFromVector(XMLoadFloat3(&light.Position)));
				pParamManager->SetWorldMatrix(&XMLoadFloat4x4(&m_WorldMatrix));

				// Render back-faces, unless we intersect with the far clip plane
				if(intersectsFarPlane)
				{
					pEffect->m_iRasterizerState = m_iBackFaceCullRSState;
					pEffect->m_iDepthStencilState = m_iLessThanDSState;
				}
				else
				{
					pEffect->m_iRasterizerState = m_iFrontFaceCullRSState;
					pEffect->m_iDepthStencilState = m_iGreaterThanDSState;
				}
				
				if(light.Type == Spot)
					pPipelineManager->Draw(*pEffect,m_ConeGeometry,pParamManager);
				else if(light.Type == Point)
					pPipelineManager->Draw(*pEffect,m_SphereGeometry,pParamManager);
			}
		}
	}

	pPipelineManager->ClearPipelineResources();

	// Clear the lights
	
	m_Lights.clear();
}
//--------------------------------------------------------------------------------
void ViewLights::Resize(UINT width,UINT height)
{
	// Nothing needed here, since the main render view handles the resizing of
	// the resources and the viewports.
}
//--------------------------------------------------------------------------------
void ViewLights::SetRenderParams(IParameterManager* pParamManager)
{
	pParamManager->SetViewMatrix(&ViewMatrix);
	pParamManager->SetProjectionMatrix(&ProjMatrix);

	XMMATRIX inv;
	XMVECTOR det;
	inv = XMMatrixInverse(&det,ProjMatrix);

	pParamManager->SetMatrixParameter(m_pInvProjMatrix,&inv);
	pParamManager->SetMatrixParameter(m_pProjMatrix,&ProjMatrix);

	XMMATRIX testView = XMMatrixLookAtLH(XMVectorSet(0,10,-1,0),XMVectorSet(0,0,0,0),XMVectorSet(0,1,0,0) );
	XMMATRIX testProj = XMMatrixPerspectiveFovLH( XM_PIDIV4,1280/720,m_fNearClip,m_fFarClip);
	pParamManager->SetMatrixParameter(m_pLightViewMatrix,&inv);
	pParamManager->SetMatrixParameter(m_pLightProjMatrix,&inv);

	if(m_pScene != NULL)
	{
		XMVECTOR cameraPos(m_pScene->GetRoot()->Transform.Position());
		pParamManager->SetVectorParameter(m_pCameraPos,&cameraPos);
	}

	// Set the G-Buffer textures
	//static const std::wstring TextureNames[4] =
	//{
	//    L"NormalTexture",
	//    L"DiffuseAlbedoTexture",
	//    L"SpecularAlbedoTexture",
	//    L"PositionTexture",
	//};

	for(unsigned int i = 0; i < m_GBufferTargets.size(); ++i)
		pParamManager->SetShaderResourceParameter(m_pGBufferTargets[i],m_GBufferTargets[i]);

	// Bind depth if optimizations are enabled
	if(GBufferOptMode::Enabled())
		pParamManager->SetShaderResourceParameter(m_pDepthTexture,m_DepthTarget);
}
//--------------------------------------------------------------------------------
void ViewLights::SetUsageParams(IParameterManager* pParamManager)
{

}
//--------------------------------------------------------------------------------
void ViewLights::AddLight(const LightParams& light)
{
	m_Lights.push_back(light);
}
//--------------------------------------------------------------------------------
void ViewLights::SetTargets(std::vector<ResourcePtr>& GBufferTargets,ResourcePtr pRenderTarget,
	ResourcePtr DepthTarget,int Viewport,int vpWidth,int vpHeight)
{
	m_GBufferTargets = GBufferTargets;
	m_pRenderTarget = pRenderTarget;
	m_DepthTarget = DepthTarget;
	SetViewport(Viewport);
	m_uVPWidth = vpWidth;
	m_uVPHeight = vpHeight;
}
//--------------------------------------------------------------------------------
D3D11_RECT ViewLights::CalcScissorRect(const XMVECTOR& lightPos,float lightRange)
{
	// Create a bounding sphere for the light, based on the position and range
	XMVECTOR centerWS = lightPos;
	float radius = lightRange;

	// Transform the sphere center to view space
	XMVECTOR centerVS = XMVector3Transform(centerWS,ViewMatrix);

	// Figure out the four points at the top, bottom, left, and right of the sphere
	XMVECTOR topVS = centerVS + XMVectorSet(0.0f,radius,0.0f,0.0f);
	XMVECTOR bottomVS = centerVS - XMVectorSet(0.0f,radius,0.0f,0.0f);
	XMVECTOR leftVS = centerVS - XMVectorSet(radius,0.0f,0.0f,0.0f);
	XMVECTOR rightVS = centerVS + XMVectorSet(radius,0.0f,0.0f,0.0f);

	XMFLOAT3 topVS3,bottomVS3,leftVS3,rightVS3;
	XMStoreFloat3(&topVS3,topVS);
	XMStoreFloat3(&bottomVS3,bottomVS);
	XMStoreFloat3(&leftVS3,leftVS);
	XMStoreFloat3(&rightVS3,rightVS);

	// Figure out whether we want to use the top and right from quad
	// tangent to the front of the sphere, or the back of the sphere
	leftVS3.z = leftVS3.x < 0.0f ? leftVS3.z - radius : leftVS3.z + radius;
	rightVS3.z = rightVS3.x < 0.0f ? rightVS3.z + radius : rightVS3.z - radius;
	topVS3.z = topVS3.y < 0.0f ? topVS3.z + radius : topVS3.z - radius;
	bottomVS3.z = bottomVS3.y < 0.0f ? bottomVS3.z - radius : bottomVS3.z + radius;

	// Clamp the z coordinate to the clip planes
	leftVS3.z = Clamp(leftVS3.z,m_fNearClip,m_fFarClip);
	rightVS3.z = Clamp(rightVS3.z,m_fNearClip,m_fFarClip);
	topVS3.z = Clamp(topVS3.z,m_fNearClip,m_fFarClip);
	bottomVS3.z = Clamp(bottomVS3.z,m_fNearClip,m_fFarClip);

	//Load Projection Matrix
	XMFLOAT4X4 p;
	XMStoreFloat4x4(&p,ProjMatrix);

	// Figure out the rectangle in clip-space by applying the perspective transform.
	// We assume that the perspective transform is symmetrical with respect to X and Y.
	float rectLeftCS = leftVS3.x * p.m[0][0] / leftVS3.z;
	float rectRightCS = rightVS3.x * p.m[0][0] / rightVS3.z;
	float rectTopCS = topVS3.y * p.m[1][1] / topVS3.z;
	float rectBottomCS = bottomVS3.y * p.m[1][1]/ bottomVS3.z;

	// Clamp the rectangle to the screen extents
	rectTopCS = Clamp(rectTopCS,-1.0f,1.0f);
	rectBottomCS = Clamp(rectBottomCS,-1.0f,1.0f);
	rectLeftCS = Clamp(rectLeftCS,-1.0f,1.0f);
	rectRightCS = Clamp(rectRightCS,-1.0f,1.0f);

	// Now we convert to screen coordinates by applying the viewport transform
	float rectTopSS = rectTopCS * 0.5f + 0.5f;
	float rectBottomSS = rectBottomCS * 0.5f + 0.5f;
	float rectLeftSS = rectLeftCS * 0.5f + 0.5f;
	float rectRightSS = rectRightCS * 0.5f + 0.5f;

	rectTopSS = 1.0f - rectTopSS;
	rectBottomSS = 1.0f - rectBottomSS;

	rectTopSS *= m_uVPHeight;
	rectBottomSS *= m_uVPHeight;
	rectLeftSS *= m_uVPWidth;
	rectRightSS *= m_uVPWidth;

	// Final step is to convert to integers and fill out the D3D11_RECT structure
	D3D11_RECT rect;
	rect.left = static_cast<LONG>(rectLeftSS);
	rect.right = static_cast<LONG>(rectRightSS);
	rect.top = static_cast<LONG>(rectTopSS);
	rect.bottom = static_cast<LONG>(rectBottomSS);

	return rect;
}
//--------------------------------------------------------------------------------
void ViewLights::SetClipPlanes(float NearClip,float FarClip)
{
	m_fNearClip = NearClip;
	m_fFarClip = FarClip;
}
//--------------------------------------------------------------------------------
std::wstring ViewLights::GetName()
{
	return(L"ViewLights");
}
//--------------------------------------------------------------------------------
