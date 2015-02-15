//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RendererDX11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkLog.h"
#include "ArkConfigFile.h"
#include "DxDebugHelper.h"

#include "ArkRenderer11.h"
#include "Dx11Texture2D.h"
#include "Dx11Resource.h"
#include "Dx11SwapChain.h"
#include "Dx11Texture2DConfig.h"
#include "Dx11ShaderResourceView.h"
#include "Dx11RenderTargetView.h"
#include "Dx11DepthStencilView.h"
#include "Dx11Texture2DConfig.h"
#include "Dx11SwapChainConfig.h"
#include "Dx11ViewPort.h"
#include "PipelineManager.h"
#include "TaskCore.h"

// Library imports
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "DXGI.lib" )
//--------------------------------------------------------------------------------
using namespace Arkeng;
using namespace Microsoft::WRL;
//--------------------------------------------------------------------------------
ArkRenderer11* ArkRenderer11::m_spRenderer = 0;
//--------------------------------------------------------------------------------
ArkRenderer11::ArkRenderer11()
{
	m_spRenderer = this;
	pPipeline = 0;

}
//--------------------------------------------------------------------------------
ArkRenderer11::~ArkRenderer11()
{
}
//--------------------------------------------------------------------------------
ArkRenderer11* ArkRenderer11::Get()
{
	return m_spRenderer;
}
//--------------------------------------------------------------------------------
bool ArkRenderer11::Initialize(D3D_DRIVER_TYPE DriverType,D3D_FEATURE_LEVEL FeatureLevel)
{


	ArkLog::Get(LogType::Renderer).Output(L"Initializing Direct3D 11!");
	HRESULT hr = S_OK;

	ComPtr<IDXGIFactory1> pFactory;
	HR_CHECK(hr = CreateDXGIFactory1(__uuidof(IDXGIFactory),reinterpret_cast<void**>(pFactory.GetAddressOf())));

	ArkLog::Get(LogType::Renderer).Output(L"Enumerating D3D11 video adapters");

	std::vector<IDXGIAdapter1*> vAdapters;
	IDXGIAdapter1* pAdapter;

	for(int i=0; pFactory->EnumAdapters1(i,&pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		vAdapters.push_back(pAdapter);
		std::wstring log = L"Adapter " + std::to_wstring(i);
		log.append(L" ");
		DXGI_ADAPTER_DESC adapterDesc;
		pAdapter->GetDesc(&adapterDesc);

		log.append(adapterDesc.Description);

		ArkLog::Get(LogType::Renderer).Output(log);
	}




	DeviceContextComPtr pContext;

	UINT CreateDeviceFlags = 0;
#ifdef _DEBUG
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL level[] ={FeatureLevel};
	D3D_FEATURE_LEVEL CreatedLevel;

	if(DriverType == D3D_DRIVER_TYPE_HARDWARE)
	{
		for(auto pAdapter : vAdapters)
		{
			HR_CHECK(hr = D3D11CreateDevice(
				pAdapter,
				D3D_DRIVER_TYPE_UNKNOWN,
				nullptr,
				CreateDeviceFlags,
				level,
				1,
				D3D11_SDK_VERSION,
				m_pDevice.GetAddressOf(),
				&CreatedLevel,
				pContext.GetAddressOf()));

			//
			IDXGIOutput* pOutput = NULL;
			hr = pAdapter->EnumOutputs(0,&pOutput);

			UINT numModes = 0;
			DXGI_MODE_DESC* displayModes = 0;
			DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			hr = pOutput->GetDisplayModeList(format,0,&numModes,NULL);

			displayModes = new DXGI_MODE_DESC[numModes];

			hr = pOutput->GetDisplayModeList(format,0,&numModes,displayModes);

			ArkLog::Get(LogType::Renderer).Output(L"Renderer supported display modes:");

			for(int i=0; i < numModes; ++i)
			{
				DXGI_MODE_DESC displayMode = displayModes[i];
				std::wstring log = L"";
				log.append(std::to_wstring(displayMode.Width));
				log.append(L"x");
				log.append(std::to_wstring(displayMode.Height));

				log.append(L" ,");
				DXGI_RATIONAL ref = displayMode.RefreshRate;
				log.append(std::to_wstring(ref.Numerator/ref.Denominator));
				log.append(L" ,");

				DXGI_FORMAT fmt = displayMode.Format;
				switch(fmt)
				{
				case DXGI_FORMAT_R32G32B32A32_FLOAT:
					log.append(L"DXGI_FORMAT_R32G32B32A32_FLOAT");
					break;
				default:
					break;
				}
			}
			//

			if(hr == S_OK)
				break;
		}
	}
	else
	{
		HR_CHECK(hr = D3D11CreateDevice(
			nullptr,
			DriverType,
			nullptr,
			CreateDeviceFlags,
			level,
			1,
			D3D11_SDK_VERSION,
			m_pDevice.GetAddressOf(),
			&CreatedLevel,
			pContext.GetAddressOf()));
	}

	m_FeatureLevel = m_pDevice->GetFeatureLevel();

	m_pDeviceContext = pContext;


	pPipeline = new PipelineManager();
	pPipeline->SetDeviceContext(m_pDeviceContext,FeatureLevel);


	m_vRenderTargetViews.emplace_back(RenderTargetViewComPtr());

	hr = m_pDevice.CopyTo(m_pDebugDevice.GetAddressOf());

	return true;
}
//--------------------------------------------------------------------------------
void ArkRenderer11::Shutdown()
{
	Safe_Delete(pPipeline);

	m_vShaderResourceViews.clear();
	m_vDepthStencilViews.clear();
	m_vRenderTargetViews.clear();
	m_vViewports.clear();

	ArkLog::Get(LogType::Renderer).Output(L"Shutting down the renderer!");

	for(auto pResource : m_vResources)
		delete pResource;

	if(m_pDebugDevice != nullptr)
	{
		m_pDebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		m_pDebugDevice = nullptr;
	}

	for(auto pSwapChain : m_vSwapChains) {
		if(pSwapChain != nullptr) {
			pSwapChain->m_pSwapChain->SetFullscreenState(false,NULL);
		}
		delete pSwapChain;
	}
	m_pDeviceContext = nullptr;
	m_pDevice = nullptr;
}
//--------------------------------------------------------------------------------
void ArkRenderer11::Present(HWND hwnd,int swapchain)
{
	unsigned int index = static_cast<unsigned int>(swapchain);

	if(index < m_vSwapChains.size())
	{
		Dx11SwapChain* pSwapChain = m_vSwapChains[index];
		HRESULT hr;
		hr = pSwapChain->m_pSwapChain->Present(0,0) ;
	}
	//TODO: Log check.

}
//--------------------------------------------------------------------------------
int ArkRenderer11::CreateSwapChain(Dx11SwapChainConfig* pConfig)
{
	ComPtr<IDXGIDevice> pDXGIDevice;
	HRESULT hr = m_pDevice.CopyTo(pDXGIDevice.GetAddressOf());

	ComPtr<IDXGIAdapter> pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter),reinterpret_cast<void **>(pDXGIAdapter.GetAddressOf()));

	ComPtr<IDXGIFactory> pFactory;
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory),reinterpret_cast<void **>(pFactory.GetAddressOf()));


	ComPtr<IDXGISwapChain> pSwapChain;
	hr = pFactory->CreateSwapChain(m_pDevice.Get(),&pConfig->m_State,pSwapChain.GetAddressOf());

	if(FAILED(hr))
	{
		//Todo
		MessageBox(0,L"Can't create swapchain.",L"Arkengine",MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
		return -1;
	}

	Texture2DComPtr pSwapChainBuffer;
	hr = pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),reinterpret_cast<void**>(pSwapChainBuffer.GetAddressOf()));

	if(FAILED(hr))
	{
		//Todo
		MessageBox(0,L"Can't create swap chain buffer.",L"Arkengine",MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
		return -1;
	}

	int ResourceID = StoreNewResource(new Dx11Texture2D(pSwapChainBuffer));

	Dx11Texture2DConfig Texture2DConfig;
	pSwapChainBuffer->GetDesc(&Texture2DConfig.m_State);

	ResourcePtr Proxy(new Dx11ResourceProxy(ResourceID,&Texture2DConfig,this));



	m_vSwapChains.push_back(new Dx11SwapChain(pSwapChain,Proxy));

	return (m_vSwapChains.size() - 1);
}
//--------------------------------------------------------------------------------
int ArkRenderer11::GetUnusedResourceIndex()
{
	// Initialize return index to -1.
	int index = -1;

	// Search for a NULL index location.
	for(unsigned int i = 0; i < m_vResources.size(); i++) {
		if(m_vResources[i] == NULL) {
			index = i;
			break;
		}
	}

	// Return either an empty location, or -1 if none exist.
	return(index);
}
//--------------------------------------------------------------------------------
int ArkRenderer11::StoreNewResource(Dx11Resource* pResource)
{
	int index = GetUnusedResourceIndex();

	if(index == -1) {
		m_vResources.push_back(pResource);
		index = m_vResources.size() - 1;
	}
	else {
		m_vResources[index] = pResource;
	}

	// Shift the inner ID to the upper 16 bits.
	int innerID = (int)pResource->GetInnerID() << 16;
	index = index + innerID;

	return(index);
}
//--------------------------------------------------------------------------------
Dx11Resource* ArkRenderer11::GetResourceByIndex(int ID)
{
	Dx11Resource* pResource = 0;

	unsigned int index = ID & 0xffff;
	int innerID = (ID & 0xffff0000) >> 16;

	if(index < m_vResources.size()) {
		pResource = m_vResources[index];

		if(pResource->GetInnerID() != innerID) {
			//
		}
	}

	return(pResource);
}
//--------------------------------------------------------------------------------
ResourcePtr ArkRenderer11::GetSwapChainResource(int ID)
{
	unsigned int index = static_cast<unsigned int>(ID);

	if(index < m_vSwapChains.size())
		return(m_vSwapChains[index]->m_Resource);

	return(ResourcePtr(new Dx11ResourceProxy()));
}
//--------------------------------------------------------------------------------
void ArkRenderer11::ResizeSwapChain(int ID,UINT width,UINT height)
{
	unsigned int index = static_cast<unsigned int>(ID);

	if(!(index < m_vSwapChains.size()))
	{
		ArkLog::Get(LogType::Renderer).Write(L"Error trying to resize swap chain!");
		return;
	}

	Dx11SwapChain* pSwapChain = m_vSwapChains[index];

	Dx11Texture2D* pBackBuffer = GetTexture2DByIndex(pSwapChain->m_Resource->m_iResource);
	pBackBuffer->m_pTexture.Reset();

	Dx11RenderTargetView& RTV = m_vRenderTargetViews[pSwapChain->m_Resource->m_iResourceRTV];

	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	RTV.m_pRenderTargetView->GetDesc(&RTVDesc);
	RTV.m_pRenderTargetView.Reset();

	pPipeline->ClearPipelineState();

	HRESULT hr = pSwapChain->m_pSwapChain->ResizeBuffers(2,width,height,DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,0);

	if(FAILED(hr))
	{
		ArkLog::Get(LogType::Renderer).Write(L"Failed to resize buffers!");
	}

	hr = pSwapChain->m_pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(pBackBuffer->m_pTexture.GetAddressOf()));

	hr = m_pDevice->CreateRenderTargetView(pBackBuffer->m_pTexture.Get(),&RTVDesc,RTV.m_pRenderTargetView.GetAddressOf());
}
//--------------------------------------------------------------------------------
void ArkRenderer11::ResizeViewport(int ID,UINT width,UINT height)
{
	unsigned int index = static_cast<unsigned int>(ID);

	if(!(index < m_vViewports.size()))
	{
		ArkLog::Get(LogType::Renderer).Write(L"Error trying to resize viewport!");
	}
	Dx11ViewPort& pViewport = m_vViewports[index];

	std::wstring log = L"Resized viewport ";
	log.append( std::to_wstring(width) );
	log.append(L"x");
	log.append( std::to_wstring(height));

	
	pViewport.m_ViewPort.Width = static_cast<float>(width);
	pViewport.m_ViewPort.Height = static_cast<float>(height);

	
	
	ArkLog::Get(LogType::Renderer).Output(log);
}
//--------------------------------------------------------------------------------
ResourcePtr ArkRenderer11::CreateTexture2D(Dx11Texture2DConfig* pConfig,D3D11_SUBRESOURCE_DATA* pData,
	Dx11ShaderResourceViewConfig* pSRVConfig,
	Dx11RenderTargetViewConfig* pRTVConfig,
	Dx11DepthStencilViewConfig* pDSVConfig)
{
	Texture2DComPtr pTexture;
	HRESULT hr = m_pDevice->CreateTexture2D(&pConfig->m_State,pData,pTexture.GetAddressOf());

	if(pTexture)
	{
		Dx11Texture2D* pTex = new Dx11Texture2D(pTexture);
		pTex->SetDesiredDescription(pConfig->GetTextureDesc());
		int ResourceID = StoreNewResource(pTex);
		ResourcePtr Proxy(new Dx11ResourceProxy(ResourceID,pConfig,this,pSRVConfig,pRTVConfig,pDSVConfig));

		return Proxy;
	}
	return ResourcePtr(new Dx11ResourceProxy());
}
//--------------------------------------------------------------------------------
int ArkRenderer11::CreateRenderTargetView(int ResourceID,D3D11_RENDER_TARGET_VIEW_DESC* pDesc)
{
	ID3D11Resource* pRawResource = 0;
	Dx11Resource* pResource = GetResourceByIndex(ResourceID);

	if(pResource) {
		pRawResource = pResource->GetResource();

		if(pRawResource) {
			RenderTargetViewComPtr pView;
			HRESULT hr = m_pDevice->CreateRenderTargetView(pRawResource,pDesc,pView.GetAddressOf());

			if(pView) {
				m_vRenderTargetViews.push_back(pView);
				return(m_vRenderTargetViews.size() - 1);
			}
		}
	}

	return(-1);
}
//--------------------------------------------------------------------------------
Dx11RenderTargetView& ArkRenderer11::GetRenderTargetViewByIndex(int rid)
{
	assert(rid >= 0);
	assert(rid < m_vRenderTargetViews.size());

	return(m_vRenderTargetViews[rid]);
}
//--------------------------------------------------------------------------------
Dx11Texture2D* ArkRenderer11::GetTexture2DByIndex(int rid)
{
	Dx11Texture2D* pResult = 0;

	Dx11Resource* pResource = GetResourceByIndex(rid);

	if(pResource != NULL)
	{
		if(pResource->GetType() == RT_TEXTURE2D)
		{
			pResult = reinterpret_cast<Dx11Texture2D*> (pResource);
		}
		else
		{
			ArkLog::Get(LogType::Renderer).Write(L"Can't access the Dx11Texture2D Resource");
		}
	}
	return pResult;
}
//--------------------------------------------------------------------------------
void ArkRenderer11::QueueTask(TaskCore* pTask)
{
	m_vQueuedTasks.push_back(pTask);
}
//--------------------------------------------------------------------------------
void ArkRenderer11::ProcessTaskQueue()
{
	int size = m_vQueuedTasks.size();

	for(int i=size-1; i >= 0; i-= 1)
	{
		for(int j=0; j < 1; ++j)
		{
			int k = i-j;
			if(k >= 0)
				m_vQueuedTasks[k]->ExecuteTask(pPipeline);
		}
	}
	m_vQueuedTasks.clear();
}
//--------------------------------------------------------------------------------
int ArkRenderer11::CreateViewport(D3D11_VIEWPORT viewport)
{
	m_vViewports.push_back(viewport);

	return (m_vViewports.size() -1);
}
//--------------------------------------------------------------------------------
const Dx11ViewPort& ArkRenderer11::GetViewPort(int ID)
{
	unsigned int index = static_cast<unsigned int>(ID);

	assert(index < m_vViewports.size());

	return m_vViewports[index];

}
//--------------------------------------------------------------------------------