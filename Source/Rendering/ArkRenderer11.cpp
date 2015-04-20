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

#include "ArkVertexBuffer11.h"
#include "ArkIndexBuffer11.h"
#include "ArkConstantBuffer11.h"

#include "ArkRenderEffect11.h"
#include "ArkBuffer11Config.h"


#include "ArkShaderFactory11.h"
#include "ArkShaderReflection11.h"
#include "ArkShaderReflectionFactory11.h"

#include "ArkVertexShader11.h"
#include "ArkPixelShader11.h"
#include "ArkGeometryShader11.h"
#include "ArkDomainShader11.h"
#include "ArkComputeShader11.h"
#include "ArkHullShader11.h"

#include "ArkParameterManager11.h"

#include "Dx11Texture2D.h"
#include "Dx11Resource.h"
#include "Dx11SwapChain.h"

#include "Dx11Texture2DConfig.h"
#include "Dx11ShaderResourceView.h"
#include "Dx11RenderTargetView.h"
#include "Dx11DepthStencilView.h"
#include "Dx11UnorderedAccessView.h"
#include "Dx11Texture2DConfig.h"
#include "Dx11SwapChainConfig.h"
#include "Dx11ViewPort.h"
#include "Dx11RasterizerStateConfig.h"
#include "Dx11DepthStencilViewConfig.h"
#include "Dx11DepthStencilStateConfig.h"
#include "Dx11BlendStateConfig.h"

#include "ArkCommandList11.h"
#include "Process.h"


#include "PipelineManager.h"
#include "TaskCore.h"

#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
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

	if(m_spRenderer == 0)
		m_spRenderer = this;

	m_driverType = D3D_DRIVER_TYPE_NULL;

	m_pParamMgr = 0;
	pPipeline = 0;

	MultiThreadingConfig.SetConfiguration(true);
	MultiThreadingConfig.ApplyConfiguration();

	m_FeatureLevel = D3D_FEATURE_LEVEL_9_1;

	m_bVsyncEnabled = true;

	
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
D3D_FEATURE_LEVEL ArkRenderer11::GetAvailableFeatureLevel(D3D_DRIVER_TYPE DriverType)
{
	D3D_FEATURE_LEVEL FeatureLevel;
	HRESULT hr;

	// If the device has already been created, simply return the feature level.
	// Otherwise perform a test with null inputs to get the returned feature level
	// without creating the device.  The application can then do whatever it needs
	// to for a given feature level.

	if(m_pDevice) {
		FeatureLevel = m_pDevice->GetFeatureLevel();
	}
	else {
		hr = D3D11CreateDevice(
			nullptr,
			DriverType,
			nullptr,
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			nullptr,
			&FeatureLevel,
			nullptr);

		if(FAILED(hr)) {
			ArkLog::Get(LogType::Renderer).Output(L"Failed to determine the available hardware feature level!");
		}

	}

	return(FeatureLevel);

}
//--------------------------------------------------------------------------------
D3D_FEATURE_LEVEL ArkRenderer11::GetCurrentFeatureLevel()
{
	return m_FeatureLevel;
}
//--------------------------------------------------------------------------------
UINT64 ArkRenderer11::GetAvailableVideoMemory()
{
	// Acquire the DXGI device, then the adapter.
	// TODO: This method needs to be capable of checking on multiple adapters!

	ComPtr<IDXGIDevice> pDXGIDevice;
	ComPtr<IDXGIAdapter> pDXGIAdapter;

	HRESULT hr = m_pDevice.CopyTo(pDXGIDevice.GetAddressOf());
	pDXGIDevice->GetAdapter(pDXGIAdapter.GetAddressOf());

	// Use the adapter interface to get its description.  Then grab the available
	// video memory based on if there is dedicated or shared memory for the GPU.

	DXGI_ADAPTER_DESC AdapterDesc;
	pDXGIAdapter->GetDesc(&AdapterDesc);

	UINT64 availableVideoMem = 0;

	if(AdapterDesc.DedicatedVideoMemory)
		availableVideoMem = AdapterDesc.DedicatedVideoMemory;
	else
		availableVideoMem = AdapterDesc.SharedSystemMemory;

	return(availableVideoMem);
}
//--------------------------------------------------------------------------------
bool ArkRenderer11::Initialize(D3D_DRIVER_TYPE DriverType,D3D_FEATURE_LEVEL FeatureLevel)
{


	ArkLog::Get(LogType::Renderer).Output(L"Initializing Direct3D 11!");
	HRESULT hr = S_OK;

	ComPtr<IDXGIFactory1> pFactory;
	HR_CHECK(hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1),reinterpret_cast<void**>(pFactory.GetAddressOf())));

	ArkLog::Get(LogType::Renderer).Output(L"Enumerating D3D11 video adapters");

	std::vector<ComPtr<IDXGIAdapter1>> vAdapters;
	ComPtr<IDXGIAdapter1> pAdapter;

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
				pAdapter.Get(),
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
			ComPtr<IDXGIOutput> pOutput = NULL;
			hr = pAdapter->EnumOutputs(0,&pOutput);

			UINT numModes = 0;
			DXGI_MODE_DESC* displayModes = 0;
			DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			hr = pOutput->GetDisplayModeList(format,0,&numModes,NULL);

			displayModes = new DXGI_MODE_DESC[numModes];

			hr = pOutput->GetDisplayModeList(format,0,&numModes,displayModes);

			ArkLog::Get(LogType::Renderer).Output(L"Renderer supported display modes:");

			for(unsigned int i=0; i < numModes; ++i)
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
			delete displayModes;
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



	for(UINT sampleCount = 0; sampleCount < D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; sampleCount++)
	{
		UINT maxQualityLevel;
		hr = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,sampleCount,&maxQualityLevel);

		if(maxQualityLevel > 0)
			maxQualityLevel--;

		if(!FAILED(hr))
		{
			if(maxQualityLevel > 0)
			{
				std::wstring output = L"MSAA " + std::to_wstring(sampleCount) ;
				output.append(L"x supported with " + std::to_wstring(maxQualityLevel));
				output.append(L" quality levels.");
				ArkLog::Get(LogType::Renderer).Output(output);
			}
		}
	}


	m_pParamMgr = new ArkParameterManager11(0);
	pPipeline = new PipelineManager();
	pPipeline->SetDeviceContext(m_pDeviceContext,FeatureLevel);


	Dx11RasterizerStateConfig RasterizerState;
	pPipeline->RasterizerStage.CurrentState.RasterizerState.SetState(CreateRasterizerState(&RasterizerState));

	Dx11DepthStencilStateConfig DepthStencilState;
	pPipeline->OutputMergerStage.CurrentState.DepthStencilState.SetState(CreateDepthStencilState(&DepthStencilState));

	Dx11BlendStateConfig BlendState;
	pPipeline->OutputMergerStage.CurrentState.BlendState.SetState(CreateBlendState(&BlendState));

	m_vShaderResourceViews.emplace_back(ShaderResourceViewComPtr());
	m_vRenderTargetViews.emplace_back(RenderTargetViewComPtr());
	m_vDepthStencilViews.emplace_back(DepthStencilViewComPtr());
	m_vUnorderedAccessViews.emplace_back(UnorderedAccessViewComPtr());

	D3D11_QUERY_DESC queryDesc;
	queryDesc.Query = D3D11_QUERY_PIPELINE_STATISTICS;
	queryDesc.MiscFlags = 0;

	for(int i = 0; i < PipelineManager::NumQueries; ++i)
	{
		hr = m_pDevice->CreateQuery(&queryDesc,&pPipeline->m_Queries[i]);

		if(FAILED(hr))
		{
			ArkLog::Get(LogType::Renderer).Output(L"Unable to create a query object!");
			Shutdown();
			return(false);
		}
	}

	hr = m_pDevice.CopyTo(m_pDebugDevice.GetAddressOf());

	D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS Options;
	m_pDevice->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS,&Options,sizeof(Options));
	if(Options.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x)
		ArkLog::Get(LogType::Renderer).Write(L"Device supports compute shaders plus raw and structured buffers via shader 4.x");


	D3D11_FEATURE_DATA_THREADING ThreadingOptions;
	m_pDevice->CheckFeatureSupport(D3D11_FEATURE_THREADING,&ThreadingOptions,sizeof(ThreadingOptions));


	for(int i=0; i < NUM_THREADS; ++i)
	{
		g_aPayload[i].id = i;

		DeviceContextComPtr pDeferred;
		m_pDevice->CreateDeferredContext(0,pDeferred.GetAddressOf());

		g_aPayload[i].pPipeline = new PipelineManager();
		g_aPayload[i].pPipeline->SetDeviceContext(pDeferred,m_FeatureLevel);
		g_aPayload[i].pPipeline->RasterizerStage.CurrentState.RasterizerState.SetState(0);
		g_aPayload[i].pPipeline->OutputMergerStage.CurrentState.DepthStencilState.SetState(0);
		g_aPayload[i].pPipeline->OutputMergerStage.CurrentState.BlendState.SetState(0);

		g_aPayload[i].pList = new ArkCommandList11();

		g_aPayload[i].pParamManager = new ArkParameterManager11(i+1);
		g_aPayload[i].pParamManager->AttachParent(m_pParamMgr);

		g_aPayload[i].bComplete = true;
		g_aPayload[i].pTask = nullptr;

		g_aThreadHandles[i] = 0;
		g_aThreadHandles[i] = (HANDLE)_beginthreadex(0,0xfffff,_TaskThreadProc,&g_aPayload[i],CREATE_SUSPENDED,0);

		g_aBeginEventHandle[i] = CreateEvent(0,FALSE,FALSE,0);
		g_aEndEventHandle[i] = CreateEvent(0,FALSE,FALSE,0);

		// Start the thread up now that it has a synch object to use.
		ResumeThread(g_aThreadHandles[i]);

	}
	return true;
}
//--------------------------------------------------------------------------------
Microsoft::WRL::ComPtr<ID3D11Debug> ArkRenderer11::GetDebugDevice()
{
	return m_pDebugDevice;
}
void ArkRenderer11::Shutdown()
{
	for(int i = 0; i < NUM_THREADS; i++)
	{
		g_aPayload[i].bComplete = true;
		g_aPayload[i].pTask = nullptr;

		Safe_Delete(g_aPayload[i].pParamManager);
		Safe_Delete(g_aPayload[i].pPipeline);
		Safe_Delete(g_aPayload[i].pList);

		CloseHandle(g_aThreadHandles[i]);
		CloseHandle(g_aBeginEventHandle[i]);
		CloseHandle(g_aEndEventHandle[i]);
	}

	Safe_Delete(m_pParamMgr);
	Safe_Delete(pPipeline);


	m_vShaderResourceViews.clear();
	m_vDepthStencilViews.clear();
	m_vRenderTargetViews.clear();
	m_vRasterizerStates.clear();
	m_vSamplerStates.clear();
	m_vInputLayouts.clear();
	m_vViewports.clear();

	ArkLog::Get(LogType::Renderer).Output(L"Shutting down the renderer!");

	//for ( auto pShader : m_vShaders )
	//	delete pShader;

	for(auto pResource : m_vResources)
		pResource = nullptr;



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
		if(m_bVsyncEnabled)
			hr = pSwapChain->m_pSwapChain->Present(1,0) ;
		else
			hr = pSwapChain->m_pSwapChain->Present(1,0) ;
	}
	//TODO: Log check.

}
//--------------------------------------------------------------------------------
UINT ArkRenderer11::GetMSQualityLevels(DXGI_FORMAT format,UINT count)
{
	UINT msCount;
	HRESULT hr = m_pDevice->CheckMultisampleQualityLevels(format,count,&msCount);
	if(!FAILED(hr))
		return msCount;
	return 0;
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
		MessageBoxW(0,L"Can't create swapchain.",L"Arkengine",MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
		return -1;
	}

	Texture2DComPtr pSwapChainBuffer;
	hr = pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),reinterpret_cast<void**>(pSwapChainBuffer.GetAddressOf()));

	if(FAILED(hr))
	{
		//Todo
		MessageBoxW(0,L"Can't create swap chain buffer.",L"Arkengine",MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
		return -1;
	}

	int ResourceID = StoreNewResource(std::make_shared<Dx11Texture2D>(pSwapChainBuffer));

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
int ArkRenderer11::StoreNewResource(std::shared_ptr<Dx11Resource> pResource)
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
std::shared_ptr<Dx11Resource> ArkRenderer11::GetResourceByIndex(int ID)
{
	std::shared_ptr<Dx11Resource> pResource = 0;

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
void ArkRenderer11::ResizeTexture(ResourcePtr texture,UINT width,UINT height)
{
	// For the texture, and then for each associated resource view create the new
	// sized versions.  Afterwards, release the old versions and replace them with
	// the new ones.
	int rid = texture->m_iResource;

	// Grab the old texture description and update it for the new size.
	std::shared_ptr<Dx11Texture2D> pTexture = GetTexture2DByIndex(rid);
	D3D11_TEXTURE2D_DESC TexDesc = pTexture->GetActualDescription();
	TexDesc.Width = width;
	TexDesc.Height = height;

	// Release the old texture, and replace it with the new one.
	if(FAILED(m_pDevice->CreateTexture2D(&TexDesc,0,pTexture->m_pTexture.ReleaseAndGetAddressOf()))) {
		ArkLog::Get(LogType::Renderer).Output(L"Error trying to resize texture...");
	}

	// Update the description of the texture for future reference.
	pTexture->m_ActualDesc = TexDesc;
	pTexture->m_DesiredDesc = TexDesc;
	texture->m_pTexture2dConfig->m_State = TexDesc;

	// Resize each of the resource views, if required.
	ResizeTextureSRV(rid,texture->m_iResourceSRV,width,height);
	ResizeTextureRTV(rid,texture->m_iResourceRTV,width,height);
	ResizeTextureDSV(rid,texture->m_iResourceDSV,width,height);
	ResizeTextureUAV(rid,texture->m_iResourceUAV,width,height);
}
//--------------------------------------------------------------------------------
void ArkRenderer11::ResizeTextureSRV(int RID,int SRVID,UINT width,UINT height)
{
	if(SRVID == 0) {
		return;
	}

	std::shared_ptr<Dx11Resource> pResource = GetResourceByIndex(RID);

	// Check that the input resources / views are legit.
	unsigned int index = static_cast<unsigned int>(SRVID);

	if(!pResource || !(index < m_vShaderResourceViews.size()) || (pResource->GetType() != RT_TEXTURE2D)) {
		ArkLog::Get(LogType::Renderer).Output(L"Error trying to resize a SRV!!!!");
		return;
	}

	// Get the existing UAV.
	Dx11ShaderResourceView& SRV = m_vShaderResourceViews[index];

	// Get its description.
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRV.m_pShaderResourceView->GetDesc(&SRVDesc);

	// Create the new one.
	if(FAILED(m_pDevice->CreateShaderResourceView(
		pResource->GetResource(),
		&SRVDesc,
		SRV.m_pShaderResourceView.ReleaseAndGetAddressOf())))
	{
		ArkLog::Get(LogType::Renderer).Output(L"Error trying to resize a SRV!!!!");
	}
}
//--------------------------------------------------------------------------------
void ArkRenderer11::ResizeTextureRTV(int RID,int RTVID,UINT width,UINT height)
{
	// Check to make sure we are supposed to do anything...
	if(RTVID == 0) {
		return;
	}

	std::shared_ptr<Dx11Resource> pResource = GetResourceByIndex(RID);

	// Check that the input resources / views are legit.
	unsigned int index = static_cast<unsigned int>(RTVID);

	if(!pResource || !(index < m_vRenderTargetViews.size()) || (pResource->GetType() != RT_TEXTURE2D)) {
		ArkLog::Get(LogType::Renderer).Output(L"Error trying to resize a RTV!!!!");
		return;
	}

	// Get the existing UAV.
	Dx11RenderTargetView& RTV = m_vRenderTargetViews[index];

	// Get its description.
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	RTV.m_pRenderTargetView->GetDesc(&RTVDesc);

	// Create the new one.
	if(FAILED(m_pDevice->CreateRenderTargetView(
		pResource->GetResource(),
		&RTVDesc,
		RTV.m_pRenderTargetView.ReleaseAndGetAddressOf())))
	{
		ArkLog::Get(LogType::Renderer).Output(L"Error trying to resize a RTV!!!!");
	}
}
//--------------------------------------------------------------------------------
void ArkRenderer11::ResizeTextureDSV(int RID,int DSVID,UINT width,UINT height)
{
	// Check to make sure we are supposed to do anything...
	if(DSVID == 0) {
		return;
	}

	std::shared_ptr<Dx11Resource> pResource = GetResourceByIndex(RID);

	// Check that the input resources / views are legit.
	unsigned int index = static_cast<unsigned int>(DSVID);

	if(!pResource || !(index < m_vDepthStencilViews.size()) || (pResource->GetType() != RT_TEXTURE2D)) {
		ArkLog::Get(LogType::Renderer).Output(L"Error trying to resize a DSV!!!!");
		return;
	}

	// Get the existing UAV.
	Dx11DepthStencilView& DSV = m_vDepthStencilViews[index];

	// Get its description.
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	DSV.m_pDepthStencilView->GetDesc(&DSVDesc);

	// Create the new one.
	if(FAILED(m_pDevice->CreateDepthStencilView(
		pResource->GetResource(),
		&DSVDesc,
		DSV.m_pDepthStencilView.ReleaseAndGetAddressOf())))
	{
		ArkLog::Get(LogType::Renderer).Output(L"Error trying to resize a DSV!!!!");
	}
}
//--------------------------------------------------------------------------------
void ArkRenderer11::ResizeTextureUAV(int RID,int UAVID,UINT width,UINT height)
{
	// Check to make sure we are supposed to do anything...
	if(UAVID == 0) {
		return;
	}

	std::shared_ptr<Dx11Resource> pResource = GetResourceByIndex(RID);

	// Check that the input resources / views are legit.
	unsigned int index = static_cast<unsigned int>(UAVID);

	if(!pResource || !(index < m_vUnorderedAccessViews.size()) || (pResource->GetType() != RT_TEXTURE2D)) {
		ArkLog::Get(LogType::Renderer).Output(L"Error trying to resize a UAV!!!!");
		return;
	}

	// Get the existing UAV.
	Dx11UnorderedAccessView& UAV = m_vUnorderedAccessViews[index];

	// Get its description.
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAV.m_pUnorderedAccessView->GetDesc(&UAVDesc);

	// Create the new one.
	if(FAILED(m_pDevice->CreateUnorderedAccessView(
		pResource->GetResource(),
		&UAVDesc,
		UAV.m_pUnorderedAccessView.ReleaseAndGetAddressOf())))
	{
		ArkLog::Get(LogType::Renderer).Output(L"Error trying to resize a UAV!!!!");
	}
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

	std::shared_ptr<Dx11Texture2D> pBackBuffer = GetTexture2DByIndex(pSwapChain->m_Resource->m_iResource);
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

	pViewport.m_ViewPort.Width = static_cast<float>(width);
	pViewport.m_ViewPort.Height = static_cast<float>(height);

}
//--------------------------------------------------------------------------------
ResourcePtr ArkRenderer11::CreateTexture2D(Dx11Texture2DConfig* pConfig,D3D11_SUBRESOURCE_DATA* pData,
	Dx11ShaderResourceViewConfig* pSRVConfig,
	Dx11RenderTargetViewConfig* pRTVConfig,
	Dx11UnorderedAccessViewConfig* pUAVConfig,
	Dx11DepthStencilViewConfig* pDSVConfig)
{
	Texture2DComPtr pTexture;
	HRESULT hr = m_pDevice->CreateTexture2D(&pConfig->m_State,pData,pTexture.GetAddressOf());

	if(pTexture)
	{
		std::shared_ptr<Dx11Texture2D> pTex = std::make_shared<Dx11Texture2D>(pTexture);
		pTex->SetDesiredDescription(pConfig->GetTextureDesc());
		int ResourceID = StoreNewResource(pTex);
		ResourcePtr Proxy(new Dx11ResourceProxy(ResourceID,pConfig,this,pSRVConfig,pRTVConfig,pUAVConfig,pDSVConfig));

		return Proxy;
	}
	return ResourcePtr(new Dx11ResourceProxy());
}
//--------------------------------------------------------------------------------
int ArkRenderer11::CreateRenderTargetView(int ResourceID,D3D11_RENDER_TARGET_VIEW_DESC* pDesc)
{
	ID3D11Resource* pRawResource = 0;
	std::shared_ptr<Dx11Resource> pResource = GetResourceByIndex(ResourceID);

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
int ArkRenderer11::CreateDepthStencilView(int ResourceID,D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc)
{
	ComPtr<ID3D11Resource> pRawResource = 0;
	std::shared_ptr<Dx11Resource> pResource = GetResourceByIndex(ResourceID);

	if(pResource)
	{
		pRawResource = pResource->GetResource();

		if(pRawResource)
		{

			DepthStencilViewComPtr pView;
			HRESULT hr = m_pDevice->CreateDepthStencilView(pRawResource.Get(),pDesc,pView.GetAddressOf());

			if(pView) {
				m_vDepthStencilViews.push_back(pView);
				return(m_vDepthStencilViews.size() - 1);
			}
		}
	}

	return(-1);
}
//--------------------------------------------------------------------------------
int ArkRenderer11::CreateShaderResourceView(int ResourceID,D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc)
{
	ComPtr<ID3D11Resource> pRawResource = 0;
	std::shared_ptr<Dx11Resource> pResource = GetResourceByIndex(ResourceID);

	if(pResource)
	{
		pRawResource = pResource->GetResource();

		if(pRawResource)
		{

			ShaderResourceViewComPtr pView;
			HRESULT hr = m_pDevice->CreateShaderResourceView(pRawResource.Get(),pDesc,pView.GetAddressOf());

			if(pView) {
				m_vShaderResourceViews.push_back(pView);
				return(m_vShaderResourceViews.size() - 1);
			}
		}
	}

	return(-1);
}
//--------------------------------------------------------------------------------
int ArkRenderer11::CreateUnorderedAccessView(int ResourceID,D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc)
{
	ComPtr<ID3D11Resource> pRawResource = 0;
	std::shared_ptr<Dx11Resource> pResource = GetResourceByIndex(ResourceID);

	if(pResource) {
		pRawResource = pResource->GetResource();

		if(pRawResource) {
			UnorderedAccessViewComPtr pView;
			HRESULT hr = m_pDevice->CreateUnorderedAccessView(pRawResource.Get(),pDesc,pView.GetAddressOf());

			if(pView) {
				m_vUnorderedAccessViews.push_back(pView);
				return(m_vUnorderedAccessViews.size() - 1);
			}
		}
	}

	return(-1);
}
//--------------------------------------------------------------------------------
int ArkRenderer11::CreateDepthStencilState(Dx11DepthStencilStateConfig* pConfig)
{
	DepthStencilStateComPtr pState;

	HRESULT hr = m_pDevice->CreateDepthStencilState(dynamic_cast<D3D11_DEPTH_STENCIL_DESC*>(pConfig),pState.GetAddressOf());

	if(FAILED(hr))
	{
		ArkLog::Get(LogType::Renderer).Output(L"Failed to create depth stencil state!");
		return(-1);
	}

	m_vDepthStencilStates.push_back(pState);

	return(m_vDepthStencilStates.size() - 1);
}
//--------------------------------------------------------------------------------
int ArkRenderer11::CreateRasterizerState(Dx11RasterizerStateConfig* pConfig)
{
	RasterizerStateComPtr pState;

	HRESULT hr = m_pDevice->CreateRasterizerState(dynamic_cast<D3D11_RASTERIZER_DESC*>(pConfig),pState.GetAddressOf());

	if(FAILED(hr))
	{
		ArkLog::Get(LogType::Renderer).Output(L"Failed to create rasterizer state!");
		return(-1);
	}

	m_vRasterizerStates.push_back(pState);

	return(m_vRasterizerStates.size() - 1);
}
//--------------------------------------------------------------------------------
int ArkRenderer11::CreateBlendState(Dx11BlendStateConfig* pConfig)
{
	BlendStateComPtr pState;

	HRESULT hr = m_pDevice->CreateBlendState(dynamic_cast<D3D11_BLEND_DESC*>(pConfig),pState.GetAddressOf());

	if(FAILED(hr))
	{
		ArkLog::Get(LogType::Renderer).Output(L"Failed to create blend state!");
		return(-1);
	}

	m_vBlendStates.push_back(pState);

	return(m_vBlendStates.size() - 1);
}
//--------------------------------------------------------------------------------
int ArkRenderer11::CreateSamplerState(D3D11_SAMPLER_DESC* pDesc)
{
	SamplerStateComPtr pState;

	HRESULT hr = m_pDevice->CreateSamplerState(pDesc,pState.GetAddressOf());

	if(FAILED(hr))
	{
		ArkLog::Get(LogType::Renderer).Output(L"Failed to create sampler state!");
		return(-1);
	}

	m_vSamplerStates.push_back(pState);

	return(m_vSamplerStates.size() - 1);
}
//--------------------------------------------------------------------------------
SamplerStateComPtr ArkRenderer11::GetSamplerState(int index)
{
	return(m_vSamplerStates[index]);
}
//--------------------------------------------------------------------------------
Dx11RenderTargetView& ArkRenderer11::GetRenderTargetViewByIndex(int rid)
{
	assert(rid >= 0);
	assert(rid < m_vRenderTargetViews.size());

	return(m_vRenderTargetViews[rid]);
}
//--------------------------------------------------------------------------------
Dx11DepthStencilView& ArkRenderer11::GetDepthStencilViewByIndex(int rid)
{
	assert(rid >= 0);
	assert(rid < m_vDepthStencilViews.size());

	return(m_vDepthStencilViews[rid]);
}
//--------------------------------------------------------------------------------
Dx11ShaderResourceView& ArkRenderer11::GetShaderResourceViewByIndex(int rid)
{
	assert(rid >= 0);
	assert(rid < m_vShaderResourceViews.size());

	return(m_vShaderResourceViews[rid]);
}
//--------------------------------------------------------------------------------
Dx11UnorderedAccessView& ArkRenderer11::GetUnorderedAccessViewByIndex(int rid)
{
	assert(rid >= 0);
	assert(rid < m_vUnorderedAccessViews.size());

	return(m_vUnorderedAccessViews[rid]);
}
//--------------------------------------------------------------------------------
std::shared_ptr<Dx11Texture2D> ArkRenderer11::GetTexture2DByIndex(int rid)
{
	std::shared_ptr<Dx11Texture2D> pResult = 0;

	std::shared_ptr<Dx11Resource> pResource = GetResourceByIndex(rid);

	if(pResource != NULL)
	{
		if(pResource->GetType() == RT_TEXTURE2D)
		{
			pResult = std::dynamic_pointer_cast<Dx11Texture2D> (pResource);
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
	MultiThreadingConfig.ApplyConfiguration();

	if(MultiThreadingConfig.GetConfiguration() == false)
	{
		// Single-threaded processing of the render view queue

		for(int i = m_vQueuedTasks.size()-1; i >= 0; i-=NUM_THREADS)
		{
			for(int j = 0; j < NUM_THREADS; j++)
			{
				if((i-j) >= 0)
				{
					//
					pPipeline->BeginEvent(std::wstring(L"View Draw: ") + m_vQueuedTasks[i-j]->GetName());
					m_vQueuedTasks[i-j]->ExecuteTask(pPipeline,g_aPayload[j].pParamManager);
					pPipeline->EndEvent();
					//PIXEndEvent();
				}
			}
		}

		m_vQueuedTasks.clear();
	}
	else
	{
		// Multi-threaded processing of the render view queue

		for(int i = m_vQueuedTasks.size()-1; i >= 0; i-=NUM_THREADS)
		{
			DWORD count = 0;

			for(int j = 0; j < NUM_THREADS; j++)
			{
				if((i-j) >= 0)
				{
					count++;
					g_aPayload[j].pTask = m_vQueuedTasks[i-j];
					SetEvent(g_aBeginEventHandle[j]);
				}
			}

			WaitForMultipleObjects(count,g_aEndEventHandle,true,INFINITE);

			for(int j = 0; count > 0; count--)
			{
				pPipeline->ExecuteCommandList(g_aPayload[j].pList);
				g_aPayload[j].pList->ReleaseList();
				j++;
			}

		}

		m_vQueuedTasks.clear();
	}
}
//--------------------------------------------------------------------------------
HANDLE						g_aThreadHandles[NUM_THREADS];
Arkeng::ThreadPayLoad		g_aPayload[NUM_THREADS];
HANDLE						g_aBeginEventHandle[NUM_THREADS];
HANDLE						g_aEndEventHandle[NUM_THREADS];


unsigned int WINAPI _TaskThreadProc(void* lpParameter)
{
	ThreadPayLoad* pPayload = (ThreadPayLoad*)lpParameter;

	int id = pPayload->id;


	for(; ;)
	{
		// Wait for the main thread to signal that there is a payload available.
		WaitForSingleObject(g_aBeginEventHandle[id],INFINITE);

		pPayload->pPipeline->m_pContext->ClearState();

		// Execute the render view with the provided pipeline and parameter managers.
		pPayload->pTask->ExecuteTask(pPayload->pPipeline,pPayload->pParamManager);

		// Generate the command list.
		pPayload->pPipeline->GenerateCommandList(pPayload->pList);

		// Signal to the main thread that the view has been collected into a command list.
		SetEvent(g_aEndEventHandle[id]);
	}

	return(0);
}
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
RasterizerStateComPtr ArkRenderer11::GetRasterizerState(int index)
{
	if(index >= 0)
		return(m_vRasterizerStates[index]);
	else
		return(m_vRasterizerStates[0]);
}
//--------------------------------------------------------------------------------
BlendStateComPtr ArkRenderer11::GetBlendState(int index)
{
	if(index >= 0)
		return(m_vBlendStates[index]);
	else
		return(m_vBlendStates[0]);
}
//--------------------------------------------------------------------------------
DepthStencilStateComPtr ArkRenderer11::GetDepthState(int index)
{
	if(index >= 0)
		return(m_vDepthStencilStates[index]);
	else
		return(m_vDepthStencilStates[0]);
}
//--------------------------------------------------------------------------------
InputLayoutComPtr ArkRenderer11::GetInputLayout(int ID)
{
	return m_vInputLayouts[ID];
}
//--------------------------------------------------------------------------------
int ArkRenderer11::CreateInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>& elements,int ShaderID)
{
	// Create array of elements here for the API call.
	D3D11_INPUT_ELEMENT_DESC* pElements = new D3D11_INPUT_ELEMENT_DESC[elements.size()];
	for(unsigned int i = 0; i < elements.size(); i++)
		pElements[i] = elements[i];

	// Attempt to create the input layout from the input information.
	ID3DBlob* pCompiledShader = m_vShaders[ShaderID]->m_pCompiledShader;
	InputLayoutComPtr pLayout;

	HRESULT hr = m_pDevice->CreateInputLayout(pElements,elements.size(),
		pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),pLayout.GetAddressOf());

	// Release the input elements array.
	delete[] pElements;

	// On failure, log the error and return an invalid index.
	if(FAILED(hr))
	{
		ArkLog::Get(LogType::Renderer).Write(L"Failed to create input layout!");
		return(-1);
	}

	m_vInputLayouts.push_back(pLayout);

	// Return the index for referencing later on.
	return(m_vInputLayouts.size() - 1);
}
//--------------------------------------------------------------------------------
ResourcePtr ArkRenderer11::CreateVertexBuffer(ArkBuffer11Config* pConfig,D3D11_SUBRESOURCE_DATA* pData)
{
	BufferComPtr pBuffer;
	HRESULT hr = m_pDevice->CreateBuffer(&pConfig->m_State,pData,pBuffer.GetAddressOf());

	if(pBuffer)
	{
		std::shared_ptr<ArkVertexBuffer11> pVertexBuffer = std::make_shared<ArkVertexBuffer11>(pBuffer);
		pVertexBuffer->SetDesiredDescription(pConfig->m_State);

		// Return an index with the lower 16 bits of index, and the upper
		// 16 bits to identify the resource type.

		int ResourceID = StoreNewResource(pVertexBuffer);
		ResourcePtr Proxy(new Dx11ResourceProxy(ResourceID,pConfig,this));

		return(Proxy);
	}

	return(ResourcePtr(new Dx11ResourceProxy()));
}
//--------------------------------------------------------------------------------
ResourcePtr ArkRenderer11::CreateIndexBuffer(ArkBuffer11Config* pConfig,D3D11_SUBRESOURCE_DATA* pData)
{
	// Create the buffer with the specified configuration.

	BufferComPtr pBuffer;
	HRESULT hr = m_pDevice->CreateBuffer(&pConfig->m_State,pData,pBuffer.GetAddressOf());

	if(pBuffer)
	{
		std::shared_ptr<ArkIndexBuffer11> pIndexBuffer = std::make_shared<ArkIndexBuffer11>(pBuffer);
		pIndexBuffer->SetDesiredDescription(pConfig->m_State);

		// Return an index with the lower 16 bits of index, and the upper
		// 16 bits to identify the resource type.

		int ResourceID = StoreNewResource(pIndexBuffer);
		ResourcePtr Proxy(new Dx11ResourceProxy(ResourceID,pConfig,this));

		return(Proxy);
	}

	return(ResourcePtr(new Dx11ResourceProxy()));
}
//--------------------------------------------------------------------------------
ResourcePtr ArkRenderer11::CreateConstantBuffer(ArkBuffer11Config* pConfig,D3D11_SUBRESOURCE_DATA* pData,bool bAutoUpdate)
{

	BufferComPtr pBuffer;
	HRESULT hr = m_pDevice->CreateBuffer(&pConfig->m_State,pData,pBuffer.GetAddressOf());

	if(pBuffer)
	{
		std::shared_ptr<ArkConstantBuffer11> pConstantBuffer = std::make_shared<ArkConstantBuffer11>(pBuffer);
		pConstantBuffer->SetDesiredDescription(pConfig->m_State);
		pConstantBuffer->SetAutoUpdate(bAutoUpdate);

		// Return an index with the lower 16 bits of index, and the upper
		// 16 bits to identify the resource type.
		int ResourceID = StoreNewResource(pConstantBuffer);
		ResourcePtr Proxy(new Dx11ResourceProxy(ResourceID,pConfig,this));

		return(Proxy);
	}

	return(ResourcePtr(new Dx11ResourceProxy()));
}
//--------------------------------------------------------------------------------
int ArkRenderer11::LoadShader(ShaderType type,std::wstring& filename,std::wstring& function,
	std::wstring& model,bool enablelogging)
{
	return LoadShader(type,filename,function,model,NULL,enablelogging);
}
//--------------------------------------------------------------------------------
int ArkRenderer11::LoadShader(ShaderType type,std::wstring& filename,std::wstring& function,
	std::wstring& model,const D3D_SHADER_MACRO* pDefines,bool enablelogging)
{
	for(unsigned int i = 0; i < m_vShaders.size(); i++)
	{
		ArkShader11SPtr pShader = m_vShaders[i];

		if(pShader->FileName.compare(filename) == 0
			&& pShader->Function.compare(function) == 0
			&& pShader->ShaderModel.compare(model) == 0
			&& pDefines == nullptr)
		{
			return(i);
		}
	}

	HRESULT hr = S_OK;

	ID3DBlob* pCompiledShader = NULL;

	pCompiledShader = ArkShaderFactory11::GenerateShader(type,filename,function,model,pDefines,enablelogging);
	/*pCompiledShader = ArkShaderFactory11::GeneratePrecompiledShader( filename, function, model );*/

	if(pCompiledShader == nullptr) {
		return(-1);
	}

	// Create the shader wrapper to house all of the information about its interface.

	ArkShader11SPtr pShaderWrapper = 0;

	switch(type)
	{
	case VERTEX_SHADER:
	{
		ID3D11VertexShader* pShader = 0;

		hr = m_pDevice->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			0,&pShader);

		pShaderWrapper = std::make_shared<ArkVertexShader11>(pShader);
		break;
	}

	case HULL_SHADER:
	{
		ID3D11HullShader* pShader = 0;

		hr = m_pDevice->CreateHullShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			0,&pShader);

		pShaderWrapper = std::make_shared<ArkHullShader11>(pShader);
		break;
	}

	case DOMAIN_SHADER:
	{
		ID3D11DomainShader* pShader = 0;

		hr = m_pDevice->CreateDomainShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			0,&pShader);

		pShaderWrapper = std::make_shared<ArkDomainShader11>(pShader);
		break;
	}

	case GEOMETRY_SHADER:
	{
		ID3D11GeometryShader* pShader = 0;

		hr = m_pDevice->CreateGeometryShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			0,&pShader);

		pShaderWrapper = std::make_shared<ArkGeometryShader11>(pShader);
		break;
	}

	case PIXEL_SHADER:
	{
		ID3D11PixelShader* pShader = 0;

		hr = m_pDevice->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			0,&pShader);

		pShaderWrapper = std::make_shared<ArkPixelShader11>(pShader);
		break;
	}

	case COMPUTE_SHADER:
	{
		ID3D11ComputeShader* pShader = 0;

		hr = m_pDevice->CreateComputeShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			0,&pShader);
		pShaderWrapper = std::make_shared<ArkComputeShader11>(pShader);
		break;
	}
	}

	if(FAILED(hr))
	{
		ArkLog::Get(LogType::Renderer).Output(L"Failed to create shader!");
		pCompiledShader->Release();
		return(-1);
	}


	pShaderWrapper->FileName = filename;
	pShaderWrapper->Function = function;
	pShaderWrapper->ShaderModel = model;

	m_vShaders.push_back(pShaderWrapper);

	pShaderWrapper->m_pCompiledShader = pCompiledShader;

	ArkShaderReflection11* pReflection = ArkShaderReflectionFactory11::GenerateReflection(*pShaderWrapper);


	// Initialize the constant buffers of this shader, so that they aren't 
	// lazy created later on...

	pReflection->InitializeConstantBuffers(m_pParamMgr);

	pShaderWrapper->SetReflection(pReflection);

	//pReflection->PrintShaderDetails();


	// Return the index for future referencing.

	return(m_vShaders.size() - 1);

}
//--------------------------------------------------------------------------------
ResourcePtr ArkRenderer11::LoadTexture( std::wstring filename, bool sRGB )
{
	ComPtr<ID3D11Resource> pResource;

	ArkFileSystem fs;
	filename = fs.GetTexturesDirectory() + filename;

	// Test whether this is a DDS file or not.
	std::wstring extension = filename.substr( filename.size()-3, 3 );
	std::transform( extension.begin(), extension.end(), extension.begin(), ::tolower );

	HRESULT hr = S_OK;

	if ( extension == L"dds" ) 
	{
		hr = DirectX::CreateDDSTextureFromFile(
			m_pDevice.Get(),
			filename.c_str(),
			//0,
			//D3D11_USAGE_DEFAULT,
			//D3D11_BIND_SHADER_RESOURCE,
			//0,
			//0,
			//sRGB,
			pResource.GetAddressOf(),
			nullptr );
	}
	else
	{
		hr = DirectX::CreateWICTextureFromFileEx(
			m_pDevice.Get(),
			pPipeline->m_pContext.Get(),
			filename.c_str(),
			0,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE,
			0,
			0,
			sRGB,
			pResource.GetAddressOf(),
			0 );
	}



	if ( FAILED( hr ) )
	{
		ArkLog::Get(LogType::Renderer).Output( L"Failed to load texture from file!" );
		return( ResourcePtr( new Dx11ResourceProxy() ) );
	}

	ComPtr<ID3D11Texture2D> pTexture;
	pResource.CopyTo( pTexture.GetAddressOf() );

	int ResourceID = StoreNewResource( std::make_shared<Dx11Texture2D>( pTexture ) );

	Dx11Texture2DConfig TextureConfig;
	pTexture->GetDesc( &TextureConfig.m_State );

	return( ResourcePtr( new Dx11ResourceProxy( ResourceID, &TextureConfig, this ) ) );
}
//--------------------------------------------------------------------------------
ArkShader11SPtr ArkRenderer11::GetShader(int ID)
{
	unsigned int index = static_cast<unsigned int>(ID);

	if(index < m_vShaders.size())
		return(m_vShaders[index]);
	else
		return(nullptr);
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkConstantBuffer11> ArkRenderer11::GetConstantBufferByIndex(int rid)
{
	std::shared_ptr<ArkConstantBuffer11> pResult = 0;

	std::shared_ptr<Dx11Resource> pResource = GetResourceByIndex(rid);

	if(pResource != NULL) {
		if(pResource->GetType() != RT_CONSTANTBUFFER) {
			ArkLog::Get(LogType::Renderer).Write(L"Trying to access a non-constant buffer resource!!!!");
		}
		else {
			pResult = std::dynamic_pointer_cast<ArkConstantBuffer11>(pResource);
		}
	}

	return(pResult);
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkVertexBuffer11> ArkRenderer11::GetVertexBufferByIndex(int index)
{
	std::shared_ptr<ArkVertexBuffer11> pResult = 0;
	std::shared_ptr<Dx11Resource> pResource = GetResourceByIndex(index);

	if(pResource != NULL)
	{
		if(pResource->GetType() != RT_VERTEXBUFFER)
		{
			ArkLog::Get(LogType::Renderer).Write(L"Trying to access a non-vertex buffer resource!!!!");
		}
		else
		{
			pResult = std::dynamic_pointer_cast<ArkVertexBuffer11>(pResource);
		}
	}
	return pResult;
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkIndexBuffer11> ArkRenderer11::GetIndexBufferByIndex(int index)
{
	std::shared_ptr<ArkIndexBuffer11> pResult = 0;
	std::shared_ptr<Dx11Resource> pResource = GetResourceByIndex(index);

	if(pResource != NULL)
	{
		ResourceType r = pResource->GetType();
		if(pResource->GetType() != RT_INDEXBUFFER)
		{
			ArkLog::Get(LogType::Renderer).Output(L"Trying to access a non-index buffer resource!!!!");
		}
		else
		{
			pResult = std::dynamic_pointer_cast<ArkIndexBuffer11>(pResource);
		}
	}
	return pResult;
}
//--------------------------------------------------------------------------------
void ArkRenderer11::DeleteResource(ResourcePtr ptr)
{
	DeleteResource(ptr->m_iResource);
}
//--------------------------------------------------------------------------------
void ArkRenderer11::DeleteResource(int index)
{
	std::shared_ptr<Dx11Resource> pResource = GetResourceByIndex(index);

	if(pResource != nullptr)
	{
		m_vResources[index & 0xffff] = nullptr;
	}
}