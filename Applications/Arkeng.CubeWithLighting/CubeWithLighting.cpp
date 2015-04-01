//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//App.cpp
//--------------------------------------------------------------------------------
#include "CubeWithLighting.h"
#include "Dx11RenderTargetView.h"
#include "EventManager.h"
#include "EKeyDown.h"
#include "PipelineManager.h"
#include "ArkBuffer11Config.h"
#include "IParameterManager.h"
#include "Dx11SwapChainConfig.h"
#include "Dx11Texture2DConfig.h"
#include "Dx11DepthStencilStateConfig.h"
#include "Dx11RasterizerStateConfig.h"
#include "Dx11BlendStateConfig.h"

#include "EventManager.h"
#include "EKeyDown.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};
//--------------------------------------------------------------------------------
App AppInstance;
//--------------------------------------------------------------------------------
App::App()
{

}
//--------------------------------------------------------------------------------
App::~App()
{
	m_pVertexBuffer.reset();

}
bool App::ConfigureEngineComponents()
{

	int resX = 0;
	int resY = 0;

	resX = ArkConfigFile::Get().GetConfig().DesiredResolutionX;
	resY = ArkConfigFile::Get().GetConfig().DesiredResolutionY;

	m_pWindow = new Win32RenderWindow();
	m_pWindow->SetPosition(25,25);
	m_pWindow->SetSize(resX,resY);
	m_pWindow->SetCaption(GetName());
	m_pWindow->Initialize(this);

	m_pRenderer = new ArkRenderer11();

	if(!m_pRenderer->Initialize(D3D_DRIVER_TYPE_HARDWARE,D3D_FEATURE_LEVEL_11_0))
	{


		if(!m_pRenderer->Initialize(D3D_DRIVER_TYPE_REFERENCE,D3D_FEATURE_LEVEL_10_0))
		{
			ShowWindow(m_pWindow->GetHandle(),SW_HIDE);
			MessageBox(m_pWindow->GetHandle(),L"Could not create a hardware or software Direct3D 11 device - the program will now abort!",L"Hieroglyph 3 Rendering",MB_ICONEXCLAMATION | MB_SYSTEMMODAL);

			return false;
		}

		// If using the reference device, utilize a fixed time step for any animations.

	}


	Dx11SwapChainConfig Config;
	Config.SetWidth(m_pWindow->GetWidth());
	Config.SetHeight(m_pWindow->GetHeight());
	Config.SetOutputWindow(m_pWindow->GetHandle());
	Config.SetRefreshRateDenominator(1);
	Config.SetRefreshRateNumerator(60);
	DXGI_SAMPLE_DESC SampleDesc;
	UINT msQuality = m_pRenderer->GetMSQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM,8 );
	SampleDesc.Count = 8;
	SampleDesc.Quality = 16;
	
	m_iSwapChain = m_pRenderer->CreateSwapChain(&Config);
	m_pWindow->SetSwapChain(m_iSwapChain);

	// We'll keep a copy of the render target index to use in later examples.

	m_RenderTarget = m_pRenderer->GetSwapChainResource(m_iSwapChain);

	// Next we create a depth buffer for use in the traditional rendering
	// pipeline.

	Dx11Texture2DConfig DepthConfig;
	DepthConfig.SetDepthBuffer(resX,resY);
	m_DepthTarget = m_pRenderer->CreateTexture2D(&DepthConfig,0);

	
	Dx11DepthStencilStateConfig dsConfig;
	int iDs = m_pRenderer->CreateDepthStencilState( &dsConfig );

	Dx11RasterizerStateConfig rsConfig;
	/*rsConfig.CullMode = D3D11_CULL_BACK;*/
	int iRs = m_pRenderer->CreateRasterizerState( &rsConfig );

	Dx11BlendStateConfig bConfig;
	int iBs = m_pRenderer->CreateBlendState( &bConfig );

	m_Effect.m_iBlendState = iBs;
	m_Effect.m_iDepthStencilState = iDs;
	m_Effect.m_iRasterizerState = iRs;
	m_Effect.m_uStencilRef = iDs;

	// Bind the swap chain render target and the depth buffer for use in 
	// rendering.  

	m_pRenderer->pPipeline->ClearRenderTargets();
	m_pRenderer->pPipeline->OutputMergerStage.CurrentState.RenderTargetViews.SetState(0,m_RenderTarget->m_iResourceRTV);
	m_pRenderer->pPipeline->OutputMergerStage.CurrentState.DepthTarget.SetState(m_DepthTarget->m_iResourceDSV);
	m_pRenderer->pPipeline->ApplyRenderTargets();

	m_Effect.SetVertexShader(m_pRenderer->LoadShader(VERTEX_SHADER,
		std::wstring(L"RotatingCube.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_4_0")
		));
	m_Effect.SetPixelShader(m_pRenderer->LoadShader(PIXEL_SHADER,
		std::wstring(L"RotatingCube.hlsl"),
		std::wstring(L"PSMain"),
		std::wstring(L"ps_4_0")
		));

	D3D11_INPUT_ELEMENT_DESC desc[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
	layout.push_back(desc[0]);
	layout.push_back(desc[1]);
	m_VertexLayout = m_pRenderer->CreateInputLayout(layout,m_Effect.GetVertexShader());

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(resX);
	viewport.Height = static_cast<float>(resY);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	int ViewPort = m_pRenderer->CreateViewport(viewport);
	m_pRenderer->pPipeline->RasterizerStage.CurrentState.ViewPortCount.SetState(1);
	m_pRenderer->pPipeline->RasterizerStage.CurrentState.ViewPorts.SetState(0,ViewPort);


	return true;
}
//--------------------------------------------------------------------------------
void App::ShutdownEngineComponents()
{
	Microsoft::WRL::ComPtr<ID3D11Debug> d = m_pRenderer->GetDebugDevice();
	if(m_pRenderer)
	{
		m_pRenderer->Shutdown();
		delete m_pRenderer;
	}
	if(d != nullptr)
	{
		d->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		d = nullptr;
	}
	if(m_pWindow)
	{
		m_pWindow->Shutdown();
		delete m_pWindow;
	}
}
//--------------------------------------------------------------------------------
void App::Initialize()
{
	

	Vertex vertices[] =
	{
		{DirectX::XMFLOAT3(-1.0f,1.0f,-1.0f),DirectX::XMFLOAT4(0.0f,0.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(1.0f,1.0f,-1.0f),DirectX::XMFLOAT4(0.0f,1.0f,0.0f,1.0f)},
		{DirectX::XMFLOAT3(1.0f,1.0f,1.0f),DirectX::XMFLOAT4(0.0f,1.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(-1.0f,1.0f,1.0f),DirectX::XMFLOAT4(1.0f,0.0f,0.0f,1.0f)},
		{DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f),DirectX::XMFLOAT4(1.0f,0.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(1.0f,-1.0f,-1.0f),DirectX::XMFLOAT4(1.0f,1.0f,0.0f,1.0f)},
		{DirectX::XMFLOAT3(1.0f,-1.0f,1.0f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(-1.0f,-1.0f,1.0f),DirectX::XMFLOAT4(0.0f,0.0f,0.0f,1.0f)},
	};
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertices;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	ArkBuffer11Config vbConfig;
	vbConfig.SetDefaultVertexBuffer(8 * sizeof(Vertex),false);
	m_pVertexBuffer = m_pRenderer->CreateVertexBuffer(&vbConfig,&data);
	if(m_pVertexBuffer->m_iResource == -1)
	{
		ArkLog::Get(LogType::Renderer).Output(L"Failed to create vertex buffer");
		assert(false);
	}

	UINT indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};
	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = indices;
	idata.SysMemPitch = 0;
	idata.SysMemSlicePitch = 0;

	ArkBuffer11Config ibConfig;
	ibConfig.SetDefaultIndexBuffer(sizeof(UINT) * 36,false);
	m_pIndexBuffer = m_pRenderer->CreateIndexBuffer(&ibConfig,&idata);
	if(m_pIndexBuffer->m_iResource == -1)
	{
		ArkLog::Get(LogType::Renderer).Write(L"Failed to create index buffer");
		assert(false);
	}


	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f,1.0f,-5.0f,0.0f);
	DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f,0.0f,0.0f,0.0f);
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f,1.0f,0.0f,0.0f);
	
	DirectX::XMMATRIX m_ViewMatrix = DirectX::XMMatrixLookAtLH(Eye,At,Up);
	DirectX::XMMATRIX  m_ProjMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2,(FLOAT)m_pWindow->GetWidth() / (FLOAT)m_pWindow->GetHeight(),0.01f,100.0f);

	DirectX::XMMATRIX m_WorldMatrix = DirectX::XMMatrixIdentity();
	

	m_pRenderer->m_pParamMgr->SetWorldMatrix(&m_WorldMatrix);
	m_pRenderer->m_pParamMgr->SetViewMatrix(&m_ViewMatrix);
	m_pRenderer->m_pParamMgr->SetProjectionMatrix(&m_ProjMatrix);




}
//--------------------------------------------------------------------------------
void App::Update()
{
	m_pScene->Update(1/60);
	/*m_pScene->Render(m_pRenderer);*/

	m_pRenderer->pPipeline->ClearBuffers( new float[4] { 0,0,0,0 },1.0f );

	static DWORD dwTimeStart = 0;
	static float t = 0.0f;
	DWORD dwTimeCur = GetTickCount();
	if( dwTimeStart == 0 )
		dwTimeStart = dwTimeCur;
	t = ( dwTimeCur - dwTimeStart ) / 1000.0f;

	DirectX::XMMATRIX mRotation = DirectX::XMMatrixRotationY( t );
	m_WorldMatrix = mRotation;

	m_pRenderer->m_pParamMgr->SetWorldMatrix(&m_WorldMatrix);

	UINT stride = sizeof(Vertex);
	m_pRenderer->pPipeline->Draw(m_Effect,m_pVertexBuffer,m_pIndexBuffer,m_VertexLayout,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,stride,36,m_pRenderer->m_pParamMgr);

	m_pRenderer->Present(m_pWindow->GetHandle(),m_pWindow->GetSwapChain());
}
//--------------------------------------------------------------------------------
void App::Shutdown()
{
}
//--------------------------------------------------------------------------------
bool App::HandleEvent(EventPtr pEvent)
{
	eEvent e = pEvent->GetEventType();
	if(e == SYSTEM_KEYBOARD_KEYDOWN)
	{
		EKeyDownPtr pKeyDown = std::static_pointer_cast<EKeyDown>(pEvent);
		unsigned int key = pKeyDown->GetCharacterCode();


	}
	return ArkApplication::HandleEvent(pEvent);
}
//--------------------------------------------------------------------------------
std::wstring App::GetName()
{
	return L"Basic App";
}
//--------------------------------------------------------------------------------