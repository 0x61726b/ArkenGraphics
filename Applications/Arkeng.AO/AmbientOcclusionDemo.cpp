//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RenderAppSimple.h
//--------------------------------------------------------------------------------
#include "AmbientOcclusionDemo.h"
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
#include "AOViewSettings.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
AmbientOcclusionDemo AppInstance;
//--------------------------------------------------------------------------------
AmbientOcclusionDemo::AmbientOcclusionDemo()
{
	
}
//--------------------------------------------------------------------------------
bool AmbientOcclusionDemo::ConfigureEngineComponents()
{

	int resX = 0;
	int resY = 0;

	resX = 640;
	resY = 480;

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
bool AmbientOcclusionDemo::ConfigureRenderingSetup()
{
	// Create the camera, and the render view that will produce an image of the 
	// from the camera's point of view of the scene.
	AOViewSettings vs(m_pBackBuffer,NULL);

	m_pRenderView = m_pViewManager->GetView(*m_pRenderer,EViewAmbientOcclusion,vs);
	m_pRenderView->SetBackColor( XMFLOAT4( 0.6f, 0.6f, 0.9f, 1.0f ) );

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
void AmbientOcclusionDemo::ShutdownEngineComponents()
{
	ShutdownRenderingSetup();
	ShutdownRenderingEngineComponents();
}
//--------------------------------------------------------------------------------
void AmbientOcclusionDemo::Initialize()
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

	// Create and fill the effect that will be used for this view type
	auto pEffect = std::make_shared<ArkRenderEffect11>();

	pEffect->SetVertexShader( m_pRenderer->LoadShader( VERTEX_SHADER,
		std::wstring( L"FinalVS.hlsl" ),
		std::wstring( L"VSMAIN" ),
		std::wstring( L"vs_5_0" ) ) );

	pEffect->SetPixelShader( m_pRenderer->LoadShader( PIXEL_SHADER,
		std::wstring( L"FinalPS.hlsl" ),
		std::wstring( L"PSMAIN" ),
		std::wstring( L"ps_5_0" ) ) );

	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;
	

	Dx11SamplerStateConfig SamplerConfig;
	int LinearSampler = ArkRenderer11::Get()->CreateSamplerState( &SamplerConfig );
	std::shared_ptr<ArkSamplerParameter11> pSamplerParameter = 
		ArkRenderer11::Get()->m_pParamMgr->GetSamplerStateParameterRef( std::wstring( L"DepthSampler" ) );
	pSamplerParameter->InitializeParameterData( &LinearSampler );

	XMVECTOR dragonDiffuse = XMVectorSet(0,1,0,1);
	
	

	
	

	// Create the scene and add the entities to it.  Then add the camera to the
	// scene so that it will be updated via the scene interface instead of 
	// manually manipulating it.

	MaterialPtr newMaterial = MaterialPtr( new ArkMaterial11() );
	newMaterial->Params[VT_LINEAR_DEPTH_NORMAL].bRender = true;
	newMaterial->Params[VT_LINEAR_DEPTH_NORMAL].pEffect = pDepthEffect;
	newMaterial->Params[VT_PERSPECTIVE].bRender = true;
	newMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;
	
	m_pDiffuseColorWriter = newMaterial->Parameters.SetVectorParameter( std::wstring(L"DiffuseColor"),XMVectorSet(1,1,1,1));

	Actor* mPlaneActor = new Actor();
	mPlaneActor->GetBody()->Visual.SetGeometry( pPlaneGeometry );
	mPlaneActor->GetBody()->Visual.SetMaterial( newMaterial );
	mPlaneActor->GetBody()->Transform.Position() = DirectX::XMVectorSet( 0, 0.0f, 0,0.0f );  
	mPlaneActor->GetBody()->Transform.Scale() = DirectX::XMVectorSet(10,10,10,0.0f);


	

	m_pActor = new Actor();
	m_pActor->GetBody()->Visual.SetGeometry( pGeometry );
	m_pActor->GetBody()->Visual.SetMaterial( pMaterial );
	m_pActor->GetBody()->Transform.Scale() = DirectX::XMVectorSet(0.5f,0.5f,0.5f,0.0f);
	m_pActor->GetBody()->Transform.Position() = DirectX::XMVectorSet( 0, 0.0f, -2,0.0f );  
	m_pActor->GetBody()->Transform.Rotation() = DirectX::XMMatrixRotationRollPitchYawFromVector(XMVectorSet(0, (3.14f*90)/2,0,0));
	m_pDiffuseColorWriter = pMaterial->Parameters.SetVectorParameter( std::wstring(L"DiffuseColor"),dragonDiffuse);

	m_pScene->AddActor( m_pActor );
	m_pScene->AddActor( mPlaneActor );

	// Get a handle to the render parameters that the application will be setting.
	m_pTimeFactors = m_pRenderer->m_pParamMgr->GetVectorParameterRef( std::wstring( L"TimeFactors" ) );

	XMVECTOR FinalColor = XMVectorSet( 0.5f, 1.0f, 0.5f, 1.0f );
	m_pFinalColor = m_pRenderer->m_pParamMgr->GetVectorParameterRef( std::wstring( L"FinalColor" ) );
	m_pFinalColor->InitializeParameterData( &m_pFinalColor );



	XMVECTOR LightParams = XMVectorSet( 0.66f, 0.89f, 1.0f, 1.0f );
	m_pLightColor = m_pRenderer->m_pParamMgr->GetVectorParameterRef( std::wstring( L"LightColor" ) );
	m_pLightColor->InitializeParameterData( &LightParams );

	XMVECTOR LightPosition = XMVectorSet( 10.0f, 20.0f, -20.0f, 0.0f );
	m_pLightPosition = m_pRenderer->m_pParamMgr->GetVectorParameterRef( std::wstring( L"LightPositionWS" ) );
	m_pLightPosition->InitializeParameterData( &LightPosition );

	UpdateParameters();


}
//--------------------------------------------------------------------------------
void AmbientOcclusionDemo::Update()
{
	m_pRenderer->pPipeline->StartPipelineStatistics();
	m_pTimer->Update();

	
	XMVECTOR TimeFactors = XMVectorSet( m_pTimer->Elapsed(), (float)m_pTimer->Framerate(), 
		m_pTimer->Runtime(), (float)m_pTimer->FrameCount() );

	m_pTimeFactors->InitializeParameterData( &TimeFactors );
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
void AmbientOcclusionDemo::Shutdown()
{
	
}
//--------------------------------------------------------------------------------
bool AmbientOcclusionDemo::HandleEvent(EventPtr pEvent)
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
void AmbientOcclusionDemo::UpdateParameters()
{
	float x = (float)m_iWidth;
	float y = (float)m_iHeight;
	float zn = m_pCamera->GetNearClipPlane();
	float zf = m_pCamera->GetFarClipPlane();
	float zn_normalized = zn / zf;
	float zf_normalized = zf / zf;
	float aspect = x / y;
	float fov = m_pCamera->GetFieldOfView();
	float yn = 2.0f * zn_normalized * tan( fov / 2.0f );
	float xn = aspect * yn;

	XMVECTOR Resolution = XMVectorSet( x, y, 1.0f / x, 1.0f / y );
	m_pResolution = m_pRenderer->m_pParamMgr->GetVectorParameterRef( std::wstring( L"Resolution" ) );
	m_pResolution->InitializeParameterData( (&Resolution) );

	XMVECTOR Perspective = XMVectorSet( fov, aspect, yn, xn );
	m_pPerspective = m_pRenderer->m_pParamMgr->GetVectorParameterRef( std::wstring( L"Perspective" ) );
	m_pPerspective->InitializeParameterData( &Perspective );

	XMVECTOR Frustum = XMVectorSet( zn, zf, zn_normalized, zf_normalized );
	m_pFrustum = m_pRenderer->m_pParamMgr->GetVectorParameterRef( std::wstring( L"Frustum" ) );
	m_pFrustum->InitializeParameterData( &Frustum );
}
//--------------------------------------------------------------------------------
std::wstring AmbientOcclusionDemo::GetName()
{
	return std::wstring(L"AmbientOcclusionDemo");
}
//--------------------------------------------------------------------------------
void AmbientOcclusionDemo::HandleWindowResize( HWND handle, UINT width, UINT height )
{
	ArkRenderApplication11::HandleWindowResize(handle,width,height );

	UpdateParameters();
}