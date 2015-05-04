//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//OmnidirectionalShadowMapDemo.h
//--------------------------------------------------------------------------------
#include "OmnidirectionalShadowMapDemo.h"
#include "ArkLog.h"
#include "EventManager.h"
#include "EFrameStart.h"
#include "EKeyUp.h"
#include "EKeyDown.h"
#include "Dx11SwapChainConfig.h"
#include "Dx11Texture2DConfig.h"
#include "PerspectiveView.h"
#include "ArkFirstPersonCamera.h"
#include "ArkRenderEffect11.h"
#include "ArkBuffer11Config.h"
#include "ArkGeometryGenerator11.h"
#include "ArkMaterial11.h"
#include "ViewCascadedShadowMap.h"
#include "ArkGeometryLoader11.h"
#include "ViewShadowMap.h"
#include "CSMViewSettings.h"
#include "ViewManager.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
OmnidirectionalShadowMapDemo AppInstance;
//--------------------------------------------------------------------------------
OmnidirectionalShadowMapDemo::OmnidirectionalShadowMapDemo()
{
}
//--------------------------------------------------------------------------------
bool OmnidirectionalShadowMapDemo::ConfigureEngineComponents()
{

	int resX = 0;
	int resY = 0;

	resX = ArkConfigFile::Get().GetConfig().DesiredResolutionX;
	resY = ArkConfigFile::Get().GetConfig().DesiredResolutionY;

	if(!ConfigureRenderingEngineComponents(resX,resY,D3D_FEATURE_LEVEL_11_0)) {
		return(false);
	}

	if(!ConfigureRenderingSetup()) {
		return(false);
	}

	SetMultiThreadedMode(false);

	return(true);
}
////--------------------------------------------------------------------------------
bool OmnidirectionalShadowMapDemo::ConfigureRenderingSetup()
{
	CSMViewSettings viewSettings(m_pBackBuffer,NULL);

	m_pRenderView = m_pViewManager->GetView(*m_pRenderer,ArkViewTypes::EViewOmnidirectionalShadowMap,viewSettings);
	//
	m_pCamera = new ArkFirstPersonCamera();
	m_pCamera->SetEventManager(&CameraEventHub);

	m_pTextOverlayView = new ViewTextOverlay(*m_pRenderer,m_pBackBuffer);

	m_pCamera->Spatial().SetTranslation(XMVectorSet(0.0f,5.0f,-10,0.0f));

	m_pCamera->SetCameraView(m_pRenderView);
	m_pCamera->SetProjectionParams(0.01f,40.0f,static_cast<float>(m_iWidth) / static_cast<float>(m_iHeight),DirectX::XM_PIDIV2);
	m_pCamera->SetOverlayView(m_pTextOverlayView);
	m_pScene->AddCamera(m_pCamera);

	return true;
}
//--------------------------------------------------------------------------------
void OmnidirectionalShadowMapDemo::ShutdownEngineComponents()
{
	ShutdownRenderingSetup();
	ShutdownRenderingEngineComponents();
}
//--------------------------------------------------------------------------------
void OmnidirectionalShadowMapDemo::Initialize()
{
	GeometryPtr pGeometry = ArkGeometryLoader11::loadMS3DFile2(std::wstring(L"Sample_Scene.ms3d"));
	/*bool success = pGeometry->ComputeTangentFrame();*/
	/*_ASSERT(success);*/
	pGeometry->LoadToBuffers();
	pGeometry->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/*ArkGeometryLoader11 geoLoader;
	GeometryPtr pPlaneGeometry = geoLoader.LoadFbxFile(std::wstring(L"plane.fbx"));
	pPlaneGeometry->LoadToBuffers();
	pPlaneGeometry->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
*/
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());
	auto pDepthEffect = std::make_shared<ArkRenderEffect11>();

	pDepthEffect->SetVertexShader(m_pRenderer->LoadShader(VERTEX_SHADER,
		std::wstring(L"OmnidirectionalDepthVS.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_5_0")));
	pDepthEffect->SetPixelShader(m_pRenderer->LoadShader(PIXEL_SHADER,
		std::wstring(L"OmnidirectionalDepthPS.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_5_0")));
	pDepthEffect->SetGeometryShader(m_pRenderer->LoadShader(GEOMETRY_SHADER,
		std::wstring(L"OmnidirectionalDepthGS.hlsl"),
		std::wstring(L"GSMAIN"),
		std::wstring(L"gs_5_0")));
	pMaterial->Params[VT_LINEAR_DEPTH_NORMAL].bRender = true;
	pMaterial->Params[VT_LINEAR_DEPTH_NORMAL].pEffect = pDepthEffect;

	auto pPhong = std::make_shared<ArkRenderEffect11>();
	pPhong->SetVertexShader(m_pRenderer->LoadShader(VERTEX_SHADER,
		std::wstring(L"OmnidirectionalFinalVS.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_5_0")));
	pPhong->SetPixelShader(m_pRenderer->LoadShader(PIXEL_SHADER,
		std::wstring(L"OmnidirectionalFinalPS.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_5_0")));
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pPhong;

	Actor* mActor = new Actor();
	mActor->GetBody()->Visual.SetGeometry(pGeometry);
	mActor->GetBody()->Visual.SetMaterial(pMaterial);
	mActor->GetBody()->Transform.Position() = DirectX::XMVectorSet(0,0.0f,0,0.0f);
	
	m_pScene->AddActor(mActor);
}
//--------------------------------------------------------------------------------
void OmnidirectionalShadowMapDemo::Update()
{
	m_pRenderer->pPipeline->StartPipelineStatistics();
	m_pTimer->Update();
	EvtManager.ProcessEvent(EFrameStartPtr(new EFrameStart(m_pTimer->Elapsed())));

	XMMATRIX transform = XMMatrixIdentity();
	transform *= XMMatrixTranslation(30.0f,30.0f,0.0f);
	XMFLOAT4X4 transform4;
	XMStoreFloat4x4(&transform4,transform);
	std::wstring text = L"FPS: " + std::to_wstring(m_pTimer->Framerate());
	m_pTextOverlayView->WriteText(text,transform4,XMFLOAT4(1.0f,0.0f,0.0f,1.0f));








	m_pScene->Update(m_pTimer->Elapsed());
	XMVECTOR pos = m_pCamera->GetBody()->Transform.Position();
	m_pScene->Render(m_pRenderer);


	m_pRenderer->pPipeline->EndPipelineStatistics();
	text = m_pRenderer->pPipeline->PrintPipelineStatistics();
	transform *= XMMatrixTranslation(0.0f,500,0.0f);
	XMStoreFloat4x4(&transform4,transform);
	m_pTextOverlayView->WriteText(text,transform4,XMFLOAT4(1,0,0,1));

	m_pRenderer->Present(m_pWindow->GetHandle(),m_pWindow->GetSwapChain());
}
//--------------------------------------------------------------------------------
void OmnidirectionalShadowMapDemo::Shutdown()
{
}
//--------------------------------------------------------------------------------
bool OmnidirectionalShadowMapDemo::HandleEvent(EventPtr pEvent)
{
	eEvent e = pEvent->GetEventType();

	if(e == SYSTEM_KEYBOARD_KEYDOWN)
	{
		EKeyDownPtr pKeyDown = std::static_pointer_cast<EKeyDown>(pEvent);
		unsigned int key = pKeyDown->GetCharacterCode();


	}
	return ArkRenderApplication11::HandleEvent(pEvent);
}
//--------------------------------------------------------------------------------
std::wstring OmnidirectionalShadowMapDemo::GetName()
{
	return L"OmnidirectionalShadowMapDemo";
}
