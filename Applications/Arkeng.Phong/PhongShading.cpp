//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RenderAppSimple.h
//--------------------------------------------------------------------------------
#include "PhongShading.h"
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
PhongShading AppInstance;
//--------------------------------------------------------------------------------
PhongShading::PhongShading()
{
	
}
//--------------------------------------------------------------------------------
bool PhongShading::ConfigureEngineComponents()
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

	SetMultiThreadedMode( false );

	return(true);
}
////--------------------------------------------------------------------------------
bool PhongShading::ConfigureRenderingSetup()
{
	PerspectiveView* pPerspView = new PerspectiveView(*m_pRenderer,m_pBackBuffer);
	m_pRenderView = pPerspView;

	m_pTextOverlayView = new ViewTextOverlay( *m_pRenderer, m_pBackBuffer );
	
	m_pCamera = new ArkFirstPersonCamera();
	m_pCamera->SetEventManager(&CameraEventHub);


	m_pCamera->Spatial().SetTranslation(XMVectorSet(10,2.5f,-2,0.0f));


	m_pCamera->SetCameraView(m_pRenderView);
	m_pCamera->SetOverlayView(m_pTextOverlayView);
	m_pCamera->SetProjectionParams(0.1f,100.0f,static_cast<float>(m_iWidth) / static_cast<float>(m_iHeight),DirectX::XM_PIDIV2);

	m_pScene->AddCamera(m_pCamera);


	return true;
}
//--------------------------------------------------------------------------------
void PhongShading::ShutdownEngineComponents()
{
	ShutdownRenderingSetup();
	ShutdownRenderingEngineComponents();
}
//--------------------------------------------------------------------------------
void PhongShading::Initialize()
{
	//Create material used for the spheres

	m_Effect.SetVertexShader(m_pRenderer->LoadShader(VERTEX_SHADER,
		std::wstring(L"PhongShading.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_4_0")
		));
	m_Effect.SetPixelShader(m_pRenderer->LoadShader(PIXEL_SHADER,
		std::wstring(L"PhongShading.hlsl"),
		std::wstring(L"PSMain"),
		std::wstring(L"ps_4_0")
		));

	m_pMaterial = MaterialPtr(new ArkMaterial11());

	m_pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	m_pMaterial->Params[VT_PERSPECTIVE].pEffect = std::make_shared<ArkRenderEffect11>(m_Effect);

	//Create geometry object and actor

	GeometryPtr pGeometry = GeometryPtr(new ArkGeometry11());
	ArkGeometryGenerator11::GenerateSphere(pGeometry,50,50,1);
	pGeometry->LoadToBuffers();
	pGeometry->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


			m_pActor = new Actor();
			m_pActor->GetBody()->Visual.SetGeometry(pGeometry);
			m_pActor->GetBody()->Visual.SetMaterial(m_pMaterial);

			m_pActor->GetNode()->Transform.Position() = XMVectorSet(10,0.8f,2.0f,0.0f);
			m_pActor->GetNode()->SetName(L"Sphere");
			m_pScene->AddActor(m_pActor);



	GeometryPtr planeGeo = GeometryPtr(new ArkGeometry11());
	ArkGeometryGenerator11::GenerateTexturedPlane(planeGeo,50,50);

	planeGeo->LoadToBuffers();
	planeGeo->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pPlane = new Actor();
	m_pPlane->GetBody()->Visual.SetGeometry(planeGeo);
	m_pPlane->GetBody()->Visual.SetMaterial(m_pMaterial);
	m_pPlane->GetNode()->Transform.Position() = XMVectorSet(0.0f,0.0f,0.0f,0.0f);
	m_pScene->AddActor(m_pPlane);


	DirectX::XMVECTOR LightParams = DirectX::XMVectorSet(1.0f,1.0f,1.0f,1.0f);
	m_pLightColor = m_pRenderer->m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightColor"));
	m_pLightColor->SetVector(LightParams);

	DirectX::XMVECTOR LightPos = DirectX::XMVectorSet(10.0f,20.0f,-20.0f,0.0f);
	m_pLightPositionWriter = m_pRenderer->m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightPositionWS"));
	m_pLightPositionWriter->InitializeParameterData(&LightPos);



}
//--------------------------------------------------------------------------------
void PhongShading::Update()
{
	m_pRenderer->pPipeline->StartPipelineStatistics();
	m_pTimer->Update();
	EvtManager.ProcessEvent(EFrameStartPtr(new EFrameStart(m_pTimer->Elapsed())));

	XMMATRIX transform = XMMatrixIdentity();
	transform *= XMMatrixTranslation( 30.0f,30.0f,0.0f );
	XMFLOAT4X4 transform4;
    XMStoreFloat4x4( &transform4, transform );
    std::wstring text = L"FPS: " + std::to_wstring( m_pTimer->Framerate() );
    m_pTextOverlayView->WriteText( text, transform4, XMFLOAT4( 1.0f, 0.0f,0.0f, 1.0f ) );

	
	
	
	

	

	m_pScene->Update(m_pTimer->Elapsed());
	m_pScene->Render(m_pRenderer);

	m_pRenderer->pPipeline->EndPipelineStatistics();
	text = m_pRenderer->pPipeline->PrintPipelineStatistics();
	transform *= XMMatrixTranslation( 0.0f,500,0.0f );
	XMStoreFloat4x4( &transform4, transform );
	m_pTextOverlayView->WriteText( text,transform4,XMFLOAT4(1,0,0,1));
	
	m_pRenderer->Present(m_pWindow->GetHandle(),m_pWindow->GetSwapChain());

}
//--------------------------------------------------------------------------------
void PhongShading::Shutdown()
{
	
}
//--------------------------------------------------------------------------------
bool PhongShading::HandleEvent(EventPtr pEvent)
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
std::wstring PhongShading::GetName()
{
	return std::wstring(L"RenderAppSimple");
}