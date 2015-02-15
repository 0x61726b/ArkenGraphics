//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RendererDX11.h
//--------------------------------------------------------------------------------
#ifndef __RendererDX11_h__
#define __RendererDX11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "IRenderer.h"
#include "DX11ResourceProxy.h"

//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11Texture2D;
	class Dx11Resource;
	class Dx11SwapChain;

	class Dx11Texture2DConfig;
	class Dx11SwapChainConfig;

	class Dx11ShaderResourceView;
	class Dx11RenderTargetView;
	class Dx11DepthStencilView;
	
	class Dx11ViewPort;

	class PipelineManager;

	typedef Microsoft::WRL::ComPtr<ID3D11DeviceContext>			DeviceContextComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11Texture2D>				Texture2DComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		RenderTargetViewComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	ShaderResourceViewComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      DepthStencilViewComPtr;

	class TaskCore;

	enum ResourceType
	{
		RT_VERTEXBUFFER = 0x010000,
		RT_INDEXBUFFER = 0x020000,
		RT_CONSTANTBUFFER = 0x030000,
		RT_STRUCTUREDBUFFER = 0x040000,
		RT_BYTEADDRESSBUFFER = 0x050000,
		RT_INDIRECTARGSBUFFER = 0x060000,
		RT_TEXTURE1D = 0x070000,
		RT_TEXTURE2D = 0x080000,
		RT_TEXTURE3D = 0x090000
	};
	class ArkRenderer11 : public IRenderer
	{
	public:
		ArkRenderer11();
		virtual ~ArkRenderer11();

		static ArkRenderer11* Get();

		virtual bool Initialize(D3D_DRIVER_TYPE DriverType,D3D_FEATURE_LEVEL FeatureLevel);
		virtual void Shutdown();

		virtual void Present(HWND hWnd = 0,int swapchain = -1);

		int CreateSwapChain(Dx11SwapChainConfig* Config);

		ResourcePtr CreateTexture2D(Dx11Texture2DConfig* pConfig,D3D11_SUBRESOURCE_DATA* pData,
			Dx11ShaderResourceViewConfig* pSRVConfig = NULL,
			Dx11RenderTargetViewConfig* pRTVConfig = NULL,
			Dx11DepthStencilViewConfig* pDSVConfig= NULL);

		int CreateRenderTargetView(int ResourceID,D3D11_RENDER_TARGET_VIEW_DESC* pDesc);
		Dx11RenderTargetView& GetRenderTargetViewByIndex(int rid);


		void ResizeSwapChain(int ID,UINT width,UINT height);
		void ResizeViewport(int ID,UINT width,UINT height);

		void QueueTask( TaskCore* pTask );
		void ProcessTaskQueue();


		int			CreateViewport( D3D11_VIEWPORT v );
		const Dx11ViewPort&			GetViewPort( int index );


		Dx11Texture2D*		GetTexture2DByIndex( int ID );
	protected:
		static ArkRenderer11* m_spRenderer;

		Microsoft::WRL::ComPtr<ID3D11Device>	m_pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;

		Microsoft::WRL::ComPtr<ID3D11Debug>				m_pDebugDevice;

		D3D_DRIVER_TYPE							m_driverType;

		std::vector<Dx11SwapChain*>				m_vSwapChains;

		std::vector<Dx11Resource*>					m_vResources;

		std::vector<Dx11ViewPort>					m_vViewports;

		std::vector<Dx11ShaderResourceView>			m_vShaderResourceViews;
		std::vector<Dx11RenderTargetView>			m_vRenderTargetViews;
		std::vector<Dx11DepthStencilView>			m_vDepthStencilViews;


		std::vector< TaskCore* >						m_vQueuedTasks;

		D3D_FEATURE_LEVEL			m_FeatureLevel;

	public:
		PipelineManager*								pPipeline;
	public:
		Dx11Resource*				GetResourceByIndex(int index);

		int							GetUnusedResourceIndex();
		int							StoreNewResource(Dx11Resource* pResource);

		ResourcePtr					GetSwapChainResource(int ID);
	};

};

//--------------------------------------------------------------------------------
#endif
