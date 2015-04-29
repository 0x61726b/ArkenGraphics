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
#include "ViewShadowMap.h"
#include "ArkGeometryLoader11.h"
#include "ViewCSM.h"
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

	SetMultiThreadedMode(false);

	return(true);
}
////--------------------------------------------------------------------------------
bool RenderAppSimple::ConfigureRenderingSetup()
{
	ViewShadowMap* pPerspView = new ViewShadowMap(*m_pRenderer,m_pBackBuffer);
	m_pRenderView = pPerspView;
	
	m_pCamera = new ArkFirstPersonCamera();
	m_pCamera->SetEventManager(&CameraEventHub);

	m_pTextOverlayView = new ViewTextOverlay( *m_pRenderer, m_pBackBuffer );

	m_pCamera->Spatial().SetTranslation(XMVectorSet(0.0f,5.0f,-10.0f,0.0f));

	m_pCamera->SetCameraView(m_pRenderView);
	m_pCamera->SetProjectionParams(0.01f,40.0f,static_cast<float>(m_iWidth) / static_cast<float>(m_iHeight),DirectX::XM_PIDIV2);
	m_pCamera->SetOverlayView(m_pTextOverlayView);
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
	//GeometryPtr pGeometry = ArkGeometryLoader11::loadMS3DFile2( std::wstring( L"Sample_Scene.ms3d" ) );
 //   bool success = pGeometry->ComputeTangentFrame();
 //   _ASSERT( success );
	//pGeometry->LoadToBuffers();
	//pGeometry->SetPrimitiveType( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	ArkGeometryLoader11 geoLoader;
	GeometryPtr pGeometry = geoLoader.LoadFbxFile( std::wstring( L"dragon.fbx" ) );
	pGeometry->ComputeTangentFrame();
	pGeometry->LoadToBuffers();
	pGeometry->SetPrimitiveType( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	

	GeometryPtr pPlaneGeometry = geoLoader.LoadFbxFile( std::wstring(L"plane.fbx"));
	pPlaneGeometry->ComputeTangentFrame();
	pPlaneGeometry->LoadToBuffers();
	pPlaneGeometry->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	

	m_pMaterial = MaterialPtr(new ArkMaterial11());

	m_DiffuseTexture = m_pRenderer->LoadTexture(std::wstring(L"mtl_floor01_c.png") /*, &loadInfo*/);
	m_NormalMap = m_pRenderer->LoadTexture(std::wstring(L"mtl_floor01_n.png"));

	_ASSERT(m_DiffuseTexture->m_iResource != -1);
	_ASSERT(m_NormalMap->m_iResource != -1);

	 //Set the texture parameters
	m_pMaterial->Parameters.SetShaderResourceParameter(L"DiffuseMap",m_DiffuseTexture);
	m_pMaterial->Parameters.SetShaderResourceParameter(L"NormalMap",m_NormalMap);

	m_pActor = new Actor();
	m_pActor->GetBody()->Visual.SetGeometry(pPlaneGeometry);
	m_pActor->GetBody()->Visual.SetMaterial(m_pMaterial);

	m_pActor->GetNode()->Transform.Position() = XMVectorSet(0.0f,-0,0.0f,0.0f);
	m_pActor->GetNode()->Transform.Scale() = XMVectorSet(4.0f,4,4.0f,1.0f);
	
	/*m_pActor->GetNode()->Transform.Rotation() = XMMatrixRotationX(-XM_PI/2);*/
	m_pActor->GetNode()->SetName(L"Down");
	m_pScene->AddActor(m_pActor);

	MaterialPtr dragonMat = MaterialPtr(new ArkMaterial11());

	m_DiffuseTexture = nullptr;
	m_DiffuseTexture = m_pRenderer->LoadTexture(std::wstring(L"snow1.jpg"));
	_ASSERT(m_DiffuseTexture->m_iResource != -1);

	dragonMat->Parameters.SetShaderResourceParameter(L"DiffuseMap",m_DiffuseTexture);

	Actor* dragonActor = new Actor();
	dragonActor->GetBody()->Visual.SetGeometry(pGeometry);
	dragonActor->GetBody()->Visual.SetMaterial(dragonMat);

	dragonActor->GetNode()->Transform.Position() = XMVectorSet(0.0f,-0,0.0f,0.0f);
	dragonActor->GetNode()->Transform.Scale() = XMVectorSet(0.5f,0.5f,0.5f,1.0f);

	Actor* dragonActor2 = new Actor();
	dragonActor2->GetBody()->Visual.SetGeometry(pGeometry);
	dragonActor2->GetBody()->Visual.SetMaterial(dragonMat);

	dragonActor2->GetNode()->Transform.Position() = XMVectorSet(-10.0f,-0,0.0f,0.0f);
	dragonActor2->GetNode()->Transform.Scale() = XMVectorSet(0.5f,0.5f,0.5f,1.0f);

	Actor* dragonActor3 = new Actor();
	dragonActor3->GetBody()->Visual.SetGeometry(pGeometry);
	dragonActor3->GetBody()->Visual.SetMaterial(dragonMat);

	dragonActor3->GetNode()->Transform.Position() = XMVectorSet(-5.0f,0,5.0f,1.0f);
	dragonActor3->GetNode()->Transform.Scale() = XMVectorSet(0.5f,0.5f,0.5f,1.0f);
	
	/*m_pActor->GetNode()->Transform.Rotation() = XMMatrixRotationX(-XM_PI/2);*/
	dragonActor->GetNode()->SetName(L"Down");
	m_pScene->AddActor(dragonActor);
	m_pScene->AddActor(dragonActor2);
	m_pScene->AddActor(dragonActor3);


}
//--------------------------------------------------------------------------------
void RenderAppSimple::Update()
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
	XMVECTOR pos = m_pCamera->GetBody()->Transform.Position();
	m_pScene->Render(m_pRenderer);

	
	m_pRenderer->pPipeline->EndPipelineStatistics();
	text = m_pRenderer->pPipeline->PrintPipelineStatistics();
	transform *= XMMatrixTranslation( 0.0f,500,0.0f );
	XMStoreFloat4x4( &transform4, transform );
	m_pTextOverlayView->WriteText( text,transform4,XMFLOAT4(1,0,0,1));

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

	if(e == SYSTEM_KEYBOARD_KEYDOWN)
	{
		EKeyDownPtr pKeyDown = std::static_pointer_cast<EKeyDown>(pEvent);
		unsigned int key = pKeyDown->GetCharacterCode();


	}
	return ArkRenderApplication11::HandleEvent(pEvent);
}
//--------------------------------------------------------------------------------
std::wstring RenderAppSimple::GetName()
{
	return L"RenderAppSimple";
}