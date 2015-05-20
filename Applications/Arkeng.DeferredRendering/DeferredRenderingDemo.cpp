//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RenderAppSimple.h
//--------------------------------------------------------------------------------
#include "DeferredRenderingDemo.h"
#include "ArkLog.h"
#include "EventManager.h"
#include "EFrameStart.h"
#include "EKeyUp.h"
#include "EKeyDown.h"

#include "Dx11SwapChainConfig.h"
#include "Dx11Texture2dConfig.h"

#include "ArkGeometryLoader11.h"
#include "ArkGeometryGenerator11.h"
#include "ArkMaterialGenerator11.h"
#include "Dx11RasterizerStateConfig.h"

#include "ViewDeferredRenderer.h"
#include "ArkFirstPersonCamera.h"

#include "ArkNodedGeometryExecutor11.h"

#include "IParameterManager.h"
#include "ArkSamplerParameterWriter11.h"
#include "ArkShaderResourceParameterWriter11.h"
#include "Dx11DepthStencilStateConfig.h"

#include "Dx11DepthStencilViewConfig.h"
#include "Dx11ShaderResourceViewConfig.h"


//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
DeferredRenderingDemo AppInstance;
//--------------------------------------------------------------------------------
DeferredRenderingDemo::DeferredRenderingDemo()
{

}
//--------------------------------------------------------------------------------
bool DeferredRenderingDemo::ConfigureEngineComponents()
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
bool DeferredRenderingDemo::ConfigureRenderingSetup()
{

	// Create the camera, and the render view that will produce an image of the 
	// from the camera's point of view of the scene.

	ViewDeferredRenderer* pDeferredView	= new ViewDeferredRenderer(*m_pRenderer,m_pBackBuffer);
	m_pRenderView = pDeferredView;


	m_pTextOverlayView = new ViewTextOverlay(*m_pRenderer,m_pBackBuffer);


	m_pCamera = new ArkFirstPersonCamera();
	m_pCamera->Spatial().SetRotation(XMVectorSet(0.407f,-0.707f,0.0f,0.0f));
	m_pCamera->Spatial().SetTranslation(XMVectorSet(4.0f,4.5f,-4.0f,0.0f));

	m_pCamera->SetCameraView(m_pRenderView);
	m_pCamera->SetOverlayView(m_pTextOverlayView);
	m_pCamera->SetEventManager(&CameraEventHub);
	const float nearClip = 0.01f;
	const float farClip = 1000;

	m_pCamera->SetProjectionParams(nearClip,farClip,static_cast<float>(m_iWidth) / static_cast<float>(m_iHeight),static_cast<float>(XM_PI) / 2.0f);
	pDeferredView->SetClipPlanes(nearClip,farClip);

	m_pScene->AddCamera(m_pCamera);

	return(true);
}
//--------------------------------------------------------------------------------
void DeferredRenderingDemo::ShutdownEngineComponents()
{
	ShutdownRenderingSetup();
	ShutdownRenderingEngineComponents();
}
//--------------------------------------------------------------------------------
void DeferredRenderingDemo::Initialize()
{
	ArkGeometryLoader11 geoLoader;
	//GeometryPtr pPlane = geoLoader.LoadFbxFile(std::wstring(L"jjplane.fbx"));
	//bool success = pPlane->ComputeTangentFrame();
	//_ASSERT(success);
	//pPlane->LoadToBuffers();
	//pPlane->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	std::vector<Actor*> pCube = geoLoader.LoadFbxRecursively(std::wstring(L"Sponza2\\sponza2.fbx"));

	//ToDO: Completely redesign submesh system lmfao

	// We'll make 2 effects for filling the G-Buffer: one without optimizations, and one with
	m_pGBufferEffect[GBufferOptMode::OptDisabled] = std::make_shared<ArkRenderEffect11>();
	m_pGBufferEffect[GBufferOptMode::OptDisabled]->SetVertexShader(m_pRenderer->LoadShader(VERTEX_SHADER,
		std::wstring(L"GBuffer.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_5_0")));
	_ASSERT(m_pGBufferEffect[GBufferOptMode::OptDisabled]->GetVertexShader() != -1);

	m_pGBufferEffect[GBufferOptMode::OptDisabled]->SetPixelShader(m_pRenderer->LoadShader(PIXEL_SHADER,
		std::wstring(L"GBuffer.hlsl"),
		std::wstring(L"PSMain"),
		std::wstring(L"ps_5_0")));
	_ASSERT(m_pGBufferEffect[GBufferOptMode::OptDisabled]->GetPixelShader() != -1);

	m_pGBufferEffect[GBufferOptMode::OptEnabled] = std::make_shared<ArkRenderEffect11>();
	m_pGBufferEffect[GBufferOptMode::OptEnabled]->SetVertexShader(m_pRenderer->LoadShader(VERTEX_SHADER,
		std::wstring(L"GBuffer.hlsl"),
		std::wstring(L"VSMainOptimized"),
		std::wstring(L"vs_5_0")));
	_ASSERT(m_pGBufferEffect[GBufferOptMode::OptEnabled]->GetVertexShader() != -1);

	m_pGBufferEffect[GBufferOptMode::OptEnabled]->SetPixelShader(m_pRenderer->LoadShader(PIXEL_SHADER,
		std::wstring(L"GBuffer.hlsl"),
		std::wstring(L"PSMainOptimized"),
		std::wstring(L"ps_5_0")));
	_ASSERT(m_pGBufferEffect[GBufferOptMode::OptEnabled]->GetPixelShader() != -1);

	// Create a depth stencil state for the G-Buffer material
	// Create the depth stencil view. We'll enable depth writes and depth tests.
	// We'll also enable stencil writes, and set the stencil buffer to 1 for each pixel rendered.
	Dx11DepthStencilStateConfig dsConfig;
	dsConfig.DepthEnable = true;
	dsConfig.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsConfig.DepthFunc = D3D11_COMPARISON_LESS;
	dsConfig.StencilEnable = true;
	dsConfig.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsConfig.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsConfig.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsConfig.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsConfig.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dsConfig.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsConfig.BackFace = dsConfig.FrontFace;

	m_iGBufferDSState = m_pRenderer->CreateDepthStencilState(&dsConfig);

	if(m_iGBufferDSState == -1)
		ArkLog::Get(LogType::Renderer).Output(L"Failed to create G-Buffer depth stencil state");


	// Create a rasterizer state with back-face culling enabled
	Dx11RasterizerStateConfig rsConfig;
	rsConfig.MultisampleEnable = TRUE;
	rsConfig.CullMode = D3D11_CULL_BACK;
	m_iGBufferRSState = m_pRenderer->CreateRasterizerState(&rsConfig);

	if(m_iGBufferRSState == -1)
		ArkLog::Get(LogType::Renderer).Output(L"Failed to create G-Buffer rasterizer state");

	m_pGBufferEffect[GBufferOptMode::OptDisabled]->m_iDepthStencilState = m_iGBufferDSState;
	m_pGBufferEffect[GBufferOptMode::OptDisabled]->m_uStencilRef = 1;
	m_pGBufferEffect[GBufferOptMode::OptDisabled]->m_iRasterizerState = m_iGBufferRSState;
	m_pGBufferEffect[GBufferOptMode::OptEnabled]->m_iDepthStencilState = m_iGBufferDSState;
	m_pGBufferEffect[GBufferOptMode::OptEnabled]->m_uStencilRef = 1;
	m_pGBufferEffect[GBufferOptMode::OptEnabled]->m_iRasterizerState = m_iGBufferRSState;

	m_pMaterial = MaterialPtr(new ArkMaterial11());


	// Load textures. For the diffuse map, we'll specify that we want an sRGB format so that
	// the texture data is gamma-corrected when sampled in the shader

	// TODO: Check if DirectXTK supports this loading information!
	//
	//D3DX11_IMAGE_LOAD_INFO loadInfo;
	//loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	m_DiffuseTexture = m_pRenderer->LoadTexture(std::wstring(L"Hex.png") /*, &loadInfo*/);
	m_NormalMap = m_pRenderer->LoadTexture(std::wstring(L"8650-normal.jpg"));

	_ASSERT(m_DiffuseTexture->m_iResource != -1);
	_ASSERT(m_NormalMap->m_iResource != -1);

	// Set the texture parameters
	/*m_pMaterial->Parameters.SetShaderResourceParameter(L"DiffuseMap",m_DiffuseTexture);
	m_pMaterial->Parameters.SetShaderResourceParameter(L"NormalMap",m_NormalMap);*/

	// Create a sampler state
	D3D11_SAMPLER_DESC sampDesc;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.MaxAnisotropy = 16;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampDesc.MinLOD = 0.0f;
	sampDesc.MipLODBias = 0.0f;
	int samplerState = m_pRenderer->CreateSamplerState(&sampDesc);
	m_pMaterial->Parameters.SetSamplerParameter(L"AnisoSampler",samplerState);

	// Enable the material to render the given view type
	m_pMaterial->Params[VT_GBUFFER].bRender = true;
	m_pMaterial->Params[VT_GBUFFER].pEffect = m_pGBufferEffect[GBufferOptMode::Value];
	for(int i=0; i < pCube.size(); i++)
	{
		//GeometryPtr Geometry = GeometryPtr(pCube[i]);
		//Actor* mActor = new Actor();


		//bool success = Geometry->ComputeTangentFrame();
		//_ASSERT(success);
		//Geometry->LoadToBuffers();

		//mActor->GetBody()->Visual.SetGeometry(Geometry);
		//mActor->GetBody()->Visual.SetMaterial(m_pMaterial);
		//mActor->GetBody()->Transform.Position() = DirectX::XMVectorSet(0,-0.0f,0,0.0f);
		//mActor->GetBody()->Visual.SetGeometry(Geometry);
		ArkGeometry11* p = reinterpret_cast<ArkGeometry11*>(pCube[i]->GetBody()->Visual.GetGeometry().get());
		if(p)
		{
			p->LoadToBuffers();
			_ASSERT(p->ComputeTangentFrame());
		}
		pCube[i]->GetBody()->Visual.GetMaterial()->Parameters.SetSamplerParameter(L"AnisoSampler",samplerState);
		pCube[i]->GetBody()->Visual.GetMaterial()->Params[VT_GBUFFER].bRender = true;
		pCube[i]->GetBody()->Visual.GetMaterial()->Params[VT_GBUFFER].pEffect = m_pGBufferEffect[GBufferOptMode::Value];
		pCube[i]->GetBody()->Transform.Position() = DirectX::XMVectorSet(0,-0.0f,0,0.0f);
		pCube[i]->GetBody()->Transform.Rotation() = XMMatrixRotationX(-3.14f*0.5f);
		m_pScene->AddActor(pCube[i]);
	}


	// Create the scene and add the entities to it.  Then add the camera to the
	// scene so that it will be updated via the scene interface instead of
	// manually manipulating it.

	/*Actor* mPlane = new Actor();
	mPlane->GetBody()->Visual.SetGeometry(pPlane);
	mPlane->GetBody()->Visual.SetMaterial(m_pMaterial);
	mPlane->GetBody()->Transform.Position() = DirectX::XMVectorSet(0,-0.0f,0,0.0f);*/
	/*ArkMaterial11* p = m_pMaterial.get();
	pCube->SetMaterial(p);
	Actor* mCube = new Actor();
	mCube->GetBody()->Visual.SetGeometry(pCube);
	mCube->GetBody()->Visual.SetMaterial(m_pMaterial);
	mCube->GetBody()->Transform.Position() = DirectX::XMVectorSet(0.0f,0.6f,0,0.0f);
	*/



	/*m_pScene->AddActor(mPlane);*/
	/*m_pScene->AddActor(mCube);*/

}
//--------------------------------------------------------------------------------
void DeferredRenderingDemo::Update()
{
	// Update the timer to determine the elapsed time since last frame.  This can
	// then used for animation during the frame.

	m_pTimer->Update();

	// Create a series of time factors for use in the simulation.  The factors
	// are as follows:
	// x: Time in seconds since the last frame.
	// y: Current framerate, which is updated once per second.
	// z: Time in seconds since application startup.
	// w: Current frame number since application startup.

	XMVECTOR TimeFactors = XMVectorSet(m_pTimer->Elapsed(),(float)m_pTimer->Framerate(),
		m_pTimer->Runtime(),(float)m_pTimer->FrameCount());

	m_pRenderer->m_pParamMgr->SetVectorParameter(std::wstring(L"TimeFactors"),&TimeFactors);


	m_pMaterial->Params[VT_GBUFFER].pEffect = m_pGBufferEffect[GBufferOptMode::Value];

	// Send an event to everyone that a new frame has started.  This will be used
	// in later examples for using the material system with render views.

	EvtManager.ProcessEvent(EFrameStartPtr(new EFrameStart(m_pTimer->Elapsed())));


	// Manipulate the scene here - simply rotate the root of the scene in this
	// example.


	DrawHUD();

	// Update the scene, and then render all cameras within the scene.
	m_pScene->Update(m_pTimer->Elapsed());
	m_pScene->Render(m_pRenderer);


	// Perform the rendering and presentation for the window.
	m_pRenderer->Present(m_pWindow->GetHandle(),m_pWindow->GetSwapChain());

}
//--------------------------------------------------------------------------------
void DeferredRenderingDemo::Shutdown()
{

}
//--------------------------------------------------------------------------------
bool DeferredRenderingDemo::HandleEvent(EventPtr pEvent)
{
	eEvent e = pEvent->GetEventType();

	if(e == SYSTEM_KEYBOARD_KEYDOWN)
	{
		EKeyDownPtr pKeyDown = std::static_pointer_cast<EKeyDown>(pEvent);
		unsigned int key = pKeyDown->GetCharacterCode();

		if(key == LightOptMode::Key)
		{
			LightOptMode::Increment();
			return true;
		}

		if(key == GBufferOptMode::Key)
		{
			GBufferOptMode::Increment();
			return true;
		}

		if(key == LightMode::Key)
		{
			LightMode::Increment();
			return true;
		}

		if(key == DisplayMode::Key)
		{
			DisplayMode::Increment();
			return true;
		}

		if(key == AAMode::Key)
		{
			AAMode::Increment();
			return true;
		}
	}
	return ArkRenderApplication11::HandleEvent(pEvent);
}
//--------------------------------------------------------------------------------
std::wstring DeferredRenderingDemo::GetName()
{
	return std::wstring(L"DeferredRenderer");
}
//--------------------------------------------------------------------------------
void DeferredRenderingDemo::DrawHUD()
{
	XMMATRIX transform = XMMatrixIdentity();
	transform *= XMMatrixTranslation(30.0f,30.0f,0.0f);
	XMFLOAT4X4 transform4;
	XMStoreFloat4x4(&transform4,transform);;

	std::wstring text = L"FPS: " + std::to_wstring(m_pTimer->Framerate());
	m_pTextOverlayView->WriteText(text,transform4,XMFLOAT4(1.0f,1.0f,1.0f,1.0f));


	float x = 30.0f;
	float y = m_iHeight - 120;
	transform = XMMatrixTranslation(x,y,0.0f);
	XMStoreFloat4x4(&transform4,transform);;
	text = DisplayMode::ToString();
	m_pTextOverlayView->WriteText(text,transform4,XMFLOAT4(1.0f,1.0f,1.0f,1.0f));

	y += 20.0f;
	transform = XMMatrixTranslation(x,y,0.0f);
	XMStoreFloat4x4(&transform4,transform);;
	text = LightMode::ToString();
	m_pTextOverlayView->WriteText(text,transform4,XMFLOAT4(1.0f,1.0f,1.0f,1.0f));

	y += 20.0f;
	transform = XMMatrixTranslation(x,y,0.0f);
	XMStoreFloat4x4(&transform4,transform);;
	text = GBufferOptMode::ToString();
	m_pTextOverlayView->WriteText(text,transform4,XMFLOAT4(1.0f,1.0f,1.0f,1.0f));

	y += 20.0f;
	transform = XMMatrixTranslation(x,y,0.0f);
	XMStoreFloat4x4(&transform4,transform);;
	text = LightOptMode::ToString();
	m_pTextOverlayView->WriteText(text,transform4,XMFLOAT4(1.0f,1.0f,1.0f,1.0f));

	y += 20.0f;
	transform = XMMatrixTranslation(x,y,0.0f);
	XMStoreFloat4x4(&transform4,transform);;
	text = AAMode::ToString();
	m_pTextOverlayView->WriteText(text,transform4,XMFLOAT4(1.0f,1.0f,1.0f,1.0f));
}
//--------------------------------------------------------------------------------
void DeferredRenderingDemo::HandleWindowResize(HWND handle,UINT width,UINT height)
{
	ArkRenderApplication11::HandleWindowResize(handle,width,height);

}