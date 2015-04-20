//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RenderAppSimple.h
//--------------------------------------------------------------------------------
#include "ShadowMapDemo.h"
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
#include "ViewAmbientOcclusion.h"
#include "Dx11SamplerStateConfig.h"
#include "ArkGeometryLoader11.h"
#include "ArkParameterWriter.h"
#include "ArkParameterContainer.h"
#include "ArkVectorParameterWriter11.h"
#include "ViewShadowMap.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ShadowMapDemo AppInstance;
//--------------------------------------------------------------------------------
ShadowMapDemo::ShadowMapDemo()
{
	
}
//--------------------------------------------------------------------------------
bool ShadowMapDemo::ConfigureEngineComponents()
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
bool ShadowMapDemo::ConfigureRenderingSetup()
{
	// Create the camera, and the render view that will produce an image of the 
	// from the camera's point of view of the scene.

	m_pRenderView = new ViewShadowMap( *m_pRenderer, m_pBackBuffer );
	m_pRenderView->SetBackColor( XMFLOAT4( 0.6f, 0.6f, 0.9f, 1.0f ) );

	//Set Light matrix view
	XMMATRIX lightViewMatrix = XMMatrixLookAtLH( XMVectorSet(0,5,-5,1.0f),XMVectorSet(0,0,0,0),XMVectorSet(0,1,0,0) );
	XMMATRIX lightProj = XMMatrixPerspectiveFovLH(  static_cast<float>( XM_PI ) / 2.0f,static_cast<float>(m_iWidth) / static_cast<float>(m_iHeight),1,10 );
	reinterpret_cast<ViewShadowMap*>(m_pRenderView)->SetLightParameters(lightViewMatrix,lightProj,XMVectorSet(0,0,0,0));
	m_pTextOverlayView = new ViewTextOverlay( *m_pRenderer, m_pBackBuffer );


	m_pCamera = new ArkFirstPersonCamera();
	m_pCamera->Spatial().SetRotation( DirectX::XMVectorSet( 0, 0.0f, 0.0f,0.0f ) );
	m_pCamera->Spatial().SetTranslation( DirectX::XMVectorSet( 0.0f, 2.0f,-8.0f,0.0f ) );
	m_pCamera->SetCameraView( m_pRenderView );
	m_pCamera->SetOverlayView( m_pTextOverlayView );
	m_pCamera->SetProjectionParams( 1, 10, static_cast<float>(m_iWidth) / static_cast<float>(m_iHeight), static_cast<float>( XM_PI ) / 2.0f );
	m_pCamera->SetEventManager( &CameraEventHub );

	m_pScene->AddCamera( m_pCamera );



	return true;
}
//--------------------------------------------------------------------------------
void ShadowMapDemo::ShutdownEngineComponents()
{
	ShutdownRenderingSetup();
	ShutdownRenderingEngineComponents();
}
//--------------------------------------------------------------------------------
void ShadowMapDemo::Initialize()
{
	ArkGeometryLoader11 geoLoader;
	GeometryPtr pGeometry = geoLoader.LoadFbxFile( std::wstring( L"dragon.fbx" ) );
	pGeometry->LoadToBuffers();
	pGeometry->SetPrimitiveType( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	GeometryPtr pPlaneGeometry = geoLoader.LoadFbxFile( std::wstring(L"plane.fbx"));
	pPlaneGeometry->LoadToBuffers();
	pPlaneGeometry->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the material for use by the entity.
	MaterialPtr pMaterial = MaterialPtr( new ArkMaterial11() );

	// Create and fill the effect that will be used for this view type
	auto pDepthEffect = std::make_shared<ArkRenderEffect11>();

	pDepthEffect->SetVertexShader( m_pRenderer->LoadShader( VERTEX_SHADER,
		std::wstring( L"DepthVS.hlsl" ),
		std::wstring( L"VSMAIN" ),
		std::wstring( L"vs_5_0" ) ) );

	pDepthEffect->SetPixelShader( m_pRenderer->LoadShader( PIXEL_SHADER,
		std::wstring( L"DepthPS.hlsl" ),
		std::wstring( L"PSMAIN" ),
		std::wstring( L"ps_5_0" ) ) );

	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_LINEAR_DEPTH_NORMAL].bRender = true;
	pMaterial->Params[VT_LINEAR_DEPTH_NORMAL].pEffect = pDepthEffect;


	auto shadowMapEffect = std::make_shared<ArkRenderEffect11>();

	shadowMapEffect->SetVertexShader( m_pRenderer->LoadShader( VERTEX_SHADER,
		std::wstring( L"RenderShadowMap.hlsl" ),
		std::wstring( L"VSMAIN" ),
		std::wstring( L"vs_5_0" ) ) );

	shadowMapEffect->SetPixelShader( m_pRenderer->LoadShader( PIXEL_SHADER,
		std::wstring( L"RenderShadowMap.hlsl" ),
		std::wstring( L"PSMAIN" ),
		std::wstring( L"ps_5_0" ) ) );

	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = shadowMapEffect;

	Dx11SamplerStateConfig SamplerConfig;
	SamplerConfig.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	SamplerConfig.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerConfig.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerConfig.MipLODBias = 0.0f;
	SamplerConfig.MaxAnisotropy = 1;
	SamplerConfig.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerConfig.BorderColor[0] = 0;
	SamplerConfig.BorderColor[1] = 0;
	SamplerConfig.BorderColor[2] = 0;
	SamplerConfig.BorderColor[3] = 0;
	SamplerConfig.MinLOD = 0;
	SamplerConfig.MaxLOD = D3D11_FLOAT32_MAX;

	int SampleTypeWrap = ArkRenderer11::Get()->CreateSamplerState( &SamplerConfig );
	std::shared_ptr<ArkSamplerParameter11> pWrapSamplerParameter = 
		ArkRenderer11::Get()->m_pParamMgr->GetSamplerStateParameterRef( std::wstring( L"SampleTypeWrap" ) );
	pWrapSamplerParameter->InitializeParameterData( &SampleTypeWrap );

	SamplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	int SampleTypeClamp = ArkRenderer11::Get()->CreateSamplerState( &SamplerConfig );
	std::shared_ptr<ArkSamplerParameter11> pSamplerParameter = 
		ArkRenderer11::Get()->m_pParamMgr->GetSamplerStateParameterRef( std::wstring( L"SampleTypeClamp" ) );
	pSamplerParameter->InitializeParameterData( &SampleTypeClamp );



	
	XMVECTOR LightPosition = XMVectorSet( 0, 5, -5, 0.0f );
	m_pLightPosition = m_pRenderer->m_pParamMgr->GetVectorParameterRef( std::wstring( L"lightPosition" ) );
	m_pLightPosition->InitializeParameterData( &LightPosition );

	std::shared_ptr<ArkVectorParameter11> pLightAmbient = 
		ArkRenderer11::Get()->m_pParamMgr->GetVectorParameterRef( std::wstring( L"ambientColor") );
	pLightAmbient->InitializeParameterData( &XMVectorSet(1,1,1,1) );

	std::shared_ptr<ArkVectorParameter11> pLightDiffuse = 
		ArkRenderer11::Get()->m_pParamMgr->GetVectorParameterRef( std::wstring( L"diffuseColor") );
	pLightDiffuse->InitializeParameterData( &XMVectorSet(0,1,0,0) );

	m_pActor = new Actor();
	m_pActor->GetBody()->Visual.SetGeometry( pGeometry );
	m_pActor->GetBody()->Visual.SetMaterial( pMaterial );
	m_pActor->GetBody()->Transform.Scale() = DirectX::XMVectorSet(0.5f,0.5f,0.5f,0.0f);
	m_pActor->GetBody()->Transform.Position() = DirectX::XMVectorSet( 0, 0.0f, -2,0.0f );  
	m_pActor->GetBody()->Transform.Rotation() = DirectX::XMMatrixRotationRollPitchYawFromVector(XMVectorSet(0, (3.14f*90)/2,0,0));
	
	m_pPlaneActor = new Actor();
	m_pPlaneActor->GetBody()->Visual.SetGeometry( pPlaneGeometry );
	m_pPlaneActor->GetBody()->Visual.SetMaterial( pMaterial );
	m_pPlaneActor->GetBody()->Transform.Position() = DirectX::XMVectorSet( 0, 0.0f, 0,0.0f ); 

	m_pScene->AddActor( m_pActor );
	m_pScene->AddActor( m_pPlaneActor );
}
//--------------------------------------------------------------------------------
void ShadowMapDemo::Update()
{
	m_pRenderer->pPipeline->StartPipelineStatistics();
	m_pTimer->Update();

	
	XMVECTOR TimeFactors = XMVectorSet( m_pTimer->Elapsed(), (float)m_pTimer->Framerate(), 
		m_pTimer->Runtime(), (float)m_pTimer->FrameCount() );

	/*m_pTimeFactors->InitializeParameterData( &TimeFactors );
	EvtManager.ProcessEvent(EFrameStartPtr(new EFrameStart(m_pTimer->Elapsed())));*/

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
void ShadowMapDemo::Shutdown()
{
	
}
//--------------------------------------------------------------------------------
bool ShadowMapDemo::HandleEvent(EventPtr pEvent)
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
std::wstring ShadowMapDemo::GetName()
{
	return std::wstring(L"ShadowMapDemo");
}
//--------------------------------------------------------------------------------
void ShadowMapDemo::HandleWindowResize( HWND handle, UINT width, UINT height )
{
	ArkRenderApplication11::HandleWindowResize(handle,width,height );

}