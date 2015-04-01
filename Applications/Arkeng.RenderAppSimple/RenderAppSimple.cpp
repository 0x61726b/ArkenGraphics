//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RenderAppSimple.h
//--------------------------------------------------------------------------------
#include "RenderAppSimple.h"
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

//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};
//--------------------------------------------------------------------------------
RenderAppSimple AppInstance;
//--------------------------------------------------------------------------------
RenderAppSimple::RenderAppSimple()
{
}
//--------------------------------------------------------------------------------
bool RenderAppSimple::ConfigureEngineComponents()
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

	return(true);
}
////--------------------------------------------------------------------------------
bool RenderAppSimple::ConfigureRenderingSetup()
{
	PerspectiveView* pPerspView = new PerspectiveView(*m_pRenderer,m_pBackBuffer);
	m_pRenderView = pPerspView;

	m_pCamera = new ArkFirstPersonCamera();
	m_pCamera->SetEventManager(&CameraEventHub);

	
	m_pCamera->Spatial().SetTranslation(XMVectorSet(0.0f,0.0f,0.0f,0.0f));
	m_pCamera->SetCameraView(m_pRenderView);
	m_pCamera->SetProjectionParams(0.01f,100.0f,static_cast<float>(m_iWidth) / static_cast<float>(m_iHeight),DirectX::XM_PIDIV2);

	m_pScene->AddCamera(m_pCamera);

	return true;
}
//--------------------------------------------------------------------------------
void RenderAppSimple::ShutdownEngineComponents()
{
	ShutdownRenderingSetup();
	ShutdownRenderingEngineComponents();
}
//--------------------------------------------------------------------------------
void RenderAppSimple::Initialize()
{
	GeometryPtr pGeometry = GeometryPtr(new ArkGeometry11());
	ArkGeometryGenerator11::GenerateSphere(pGeometry,10,10,10);
	pGeometry->LoadToBuffers();
	pGeometry->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pMaterial = MaterialPtr(new ArkMaterial11());

	m_Effect.SetVertexShader(m_pRenderer->LoadShader(VERTEX_SHADER,
		std::wstring(L"SimpleApp.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_4_0")
		));
	m_Effect.SetPixelShader(m_pRenderer->LoadShader(PIXEL_SHADER,
		std::wstring(L"SimpleApp.hlsl"),
		std::wstring(L"PSMain"),
		std::wstring(L"ps_4_0")
		));

	m_pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	m_pMaterial->Params[VT_PERSPECTIVE].pEffect = std::make_shared<ArkRenderEffect11>(m_Effect);

	m_pActor = new Actor();
	m_pActor->GetBody()->Visual.SetGeometry(pGeometry);
	m_pActor->GetBody()->Visual.SetMaterial(m_pMaterial);

	m_pActor->GetNode()->Transform.Position() = XMVectorSet(0.0f,0.0f,50.0f,0.0f);
	m_pActor->GetNode()->SetName(L"Sphere");
	m_pScene->AddActor(m_pActor);

}
//--------------------------------------------------------------------------------
void RenderAppSimple::Update()
{
	m_pTimer->Update();
	EvtManager.ProcessEvent(EFrameStartPtr(new EFrameStart(m_pTimer->Elapsed())));

	DirectX::XMMATRIX mRotation = DirectX::XMMatrixRotationY(m_pTimer->Elapsed());

	//XMMATRIX rot = m_pActor->GetNode()->Transform.Rotation();
	//m_pActor->GetNode()->Transform.Rotation()= rot*mRotation;


	m_pScene->Update(m_pTimer->Elapsed());
	m_pScene->Render(m_pRenderer);

	m_pRenderer->Present(m_pWindow->GetHandle(),m_pWindow->GetSwapChain());
}
//--------------------------------------------------------------------------------
void RenderAppSimple::Shutdown()
{
}
//--------------------------------------------------------------------------------
bool RenderAppSimple::HandleEvent(EventPtr pEvent)
{
	eEvent e = pEvent->GetEventType();

	if( e == SYSTEM_KEYBOARD_KEYDOWN )
	{
		EKeyDownPtr pKeyDown = std::static_pointer_cast<EKeyDown>( pEvent );
		unsigned int key = pKeyDown->GetCharacterCode();

		
	}
	return ArkRenderApplication11::HandleEvent(pEvent);
}
//--------------------------------------------------------------------------------
std::wstring RenderAppSimple::GetName()
{
	return L"RenderAppSimple";
}