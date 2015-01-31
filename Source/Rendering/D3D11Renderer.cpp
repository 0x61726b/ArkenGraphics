//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RendererDX11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "D3D11Renderer.h"
#include "Dx11Texture2D.h"
#include "Dx11Resource.h"
#include "Dx11SwapChain.h"
#include "Dx11Texture2DConfig.h"
#include "Dx11ShaderResourceView.h"
#include "Dx11RenderTargetView.h"
#include "Dx11DepthStencilView.h"
#include "Dx11Texture2DConfig.h"
#include "Dx11SwapChainConfig.h"
#include "PipelineManager.h"

// Library imports
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "DXGI.lib" )
//--------------------------------------------------------------------------------
using namespace Arkeng;
using namespace Microsoft::WRL;
//--------------------------------------------------------------------------------
D3D11Renderer* D3D11Renderer::m_spRenderer = 0;
//--------------------------------------------------------------------------------
D3D11Renderer::D3D11Renderer()
{
	m_spRenderer = this;
	pPipeline = 0;

}
//--------------------------------------------------------------------------------
D3D11Renderer::~D3D11Renderer()
{
}
//--------------------------------------------------------------------------------
D3D11Renderer* D3D11Renderer::Get()
{
	return m_spRenderer;
}
//--------------------------------------------------------------------------------
bool D3D11Renderer::Initialize(D3D_DRIVER_TYPE DriverType,D3D_FEATURE_LEVEL FeatureLevel)
{
	HRESULT hr = S_OK;

	ComPtr<IDXGIFactory1> pFactory;
	HR_CHECK(hr = CreateDXGIFactory1(__uuidof(IDXGIFactory),reinterpret_cast<void**>(pFactory.GetAddressOf())));

	std::vector<IDXGIAdapter*> vAdapters;
	IDXGIAdapter* pAdapter;
	for(int i=0; pFactory->EnumAdapters(i,&pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		vAdapters.push_back(pAdapter);
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
	pPipeline->SetDeviceContext( m_pDeviceContext,FeatureLevel );


	m_vRenderTargetViews.emplace_back(RenderTargetViewComPtr());
	return true;
}
//--------------------------------------------------------------------------------
ID3D11DeviceContext* D3D11Renderer::GetContext()
{
	return m_pDeviceContext.Get();
}
//--------------------------------------------------------------------------------
void D3D11Renderer::Shutdown()
{

	for(auto pSwapChain : m_vSwapChains) {
		if(pSwapChain != nullptr) {
			pSwapChain->m_pSwapChain->SetFullscreenState(false,NULL);
		}
		delete pSwapChain;
	}
}
//--------------------------------------------------------------------------------
void D3D11Renderer::Present(HWND hwnd,int swapchain)
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
int D3D11Renderer::CreateSwapChain(Dx11SwapChainConfig* pConfig)
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
int D3D11Renderer::GetUnusedResourceIndex()
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
int D3D11Renderer::StoreNewResource(Dx11Resource* pResource)
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
Dx11Resource* D3D11Renderer::GetResourceByIndex(int ID)
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
ResourcePtr D3D11Renderer::GetSwapChainResource(int ID)
{
	unsigned int index = static_cast<unsigned int>(ID);

	if(index < m_vSwapChains.size())
		return(m_vSwapChains[index]->m_Resource);

	return(ResourcePtr(new Dx11ResourceProxy()));
}
//--------------------------------------------------------------------------------
void D3D11Renderer::ResizeSwapChain(int ID,UINT width,UINT height)
{

}
//--------------------------------------------------------------------------------
ResourcePtr D3D11Renderer::CreateTexture2D(Dx11Texture2DConfig* pConfig,D3D11_SUBRESOURCE_DATA* pData,
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
int D3D11Renderer::CreateRenderTargetView(int ResourceID,D3D11_RENDER_TARGET_VIEW_DESC* pDesc)
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
Dx11RenderTargetView& D3D11Renderer::GetRenderTargetViewByIndex(int rid)
{
	assert(rid >= 0);
	assert(rid < m_vRenderTargetViews.size());

	return(m_vRenderTargetViews[rid]);
}
//--------------------------------------------------------------------------------