//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkRenderer11.h
//--------------------------------------------------------------------------------
#ifndef __ArkRenderer11_h__
#define __ArkRenderer11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "IRenderer.h"
#include "TConfiguration.h"
#include "DX11ResourceProxy.h"

#include "ArkShader11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkBuffer11;
	class ArkVertexBuffer11;
	class ArkIndexBuffer11;
	class ArkConstantBuffer11;

	class ArkBuffer11Config;


	class Dx11Texture2D;
	class Dx11Resource;
	class Dx11SwapChain;

	class Dx11Texture2DConfig;
	class Dx11SwapChainConfig;

	class Dx11ShaderResourceView;
	class Dx11RenderTargetView;
	class Dx11DepthStencilView;
	class Dx11RasterizerStateConfig;
	class Dx11DepthStencilStateConfig;
	class Dx11BlendStateConfig;

	class ArkRenderEffect11;

	class Dx11ViewPort;

	class PipelineManager;
	class IParameterManager;

	class ArkCommandList11;
	class ArkGeometry11;


	typedef Microsoft::WRL::ComPtr<ID3D11Buffer>				BufferComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11DeviceContext>			DeviceContextComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11Texture2D>				Texture2DComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		RenderTargetViewComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	ShaderResourceViewComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      DepthStencilViewComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11InputLayout>			InputLayoutComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11RasterizerState>		RasterizerStateComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		DepthStencilStateComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11BlendState>			BlendStateComPtr;
	typedef Microsoft::WRL::ComPtr<ID3D11SamplerState>			SamplerStateComPtr;
	typedef std::shared_ptr<ArkShader11>						ArkShader11SPtr;

	typedef Microsoft::WRL::ComPtr<ID3D11Query>				    QueryComPtr;

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

	struct ThreadPayLoad
	{
		int id;
		bool bComplete;
		PipelineManager* pPipeline;
		IParameterManager* pParamManager;
		TaskCore* pTask;
		ArkCommandList11* pList;
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

		UINT GetMSQualityLevels( DXGI_FORMAT, UINT  );

		int CreateSwapChain(Dx11SwapChainConfig* Config);

		ResourcePtr CreateVertexBuffer(ArkBuffer11Config* pConfig,D3D11_SUBRESOURCE_DATA* pData);
		ResourcePtr CreateIndexBuffer(ArkBuffer11Config* pConfig,D3D11_SUBRESOURCE_DATA* pData);
		ResourcePtr CreateConstantBuffer(ArkBuffer11Config* pConfig,D3D11_SUBRESOURCE_DATA* pData,bool bAutoUpdate = true);

		ResourcePtr CreateTexture2D(Dx11Texture2DConfig* pConfig,D3D11_SUBRESOURCE_DATA* pData,
			Dx11ShaderResourceViewConfig* pSRVConfig = NULL,
			Dx11RenderTargetViewConfig* pRTVConfig = NULL,
			Dx11DepthStencilViewConfig* pDSVConfig= NULL);

		int CreateDepthStencilView( int ResourceID,D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc );
		int CreateShaderResourceView( int ResourceID, D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc );

		

		int CreateRenderTargetView(int ResourceID,D3D11_RENDER_TARGET_VIEW_DESC* pDesc);


		Dx11RenderTargetView&					GetRenderTargetViewByIndex(int rid);
		Dx11DepthStencilView&					GetDepthStencilViewByIndex(int rid);
		Dx11ShaderResourceView&					GetShaderResourceViewByIndex(int rid);




		int LoadShader(ShaderType type,std::wstring& filename,std::wstring& function,
			std::wstring& model,bool enablelogging = true);

		int LoadShader(ShaderType type,std::wstring& filename,std::wstring& function,
			std::wstring& model,const D3D_SHADER_MACRO* pDefines,bool enablelogging = true);

		ArkShader11SPtr GetShader(int ID);


		InputLayoutComPtr				GetInputLayout(int index);
		int CreateInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>& elements,int ShaderID);

		std::shared_ptr<ArkVertexBuffer11>				GetVertexBufferByIndex(int index);
		std::shared_ptr<ArkIndexBuffer11>				GetIndexBufferByIndex(int index);

		std::shared_ptr<ArkConstantBuffer11> GetConstantBufferByIndex(int ID);

		RasterizerStateComPtr		GetRasterizerState(int index);
		int							CreateRasterizerState( Dx11RasterizerStateConfig* Config );

		int							CreateBlendState( Dx11BlendStateConfig* Config );
		BlendStateComPtr			GetBlendState( int index );

		DepthStencilStateComPtr		GetDepthState( int index );
		int CreateDepthStencilState( Dx11DepthStencilStateConfig* pConfig );

		int CreateSamplerState( D3D11_SAMPLER_DESC* pDesc );
		SamplerStateComPtr GetSamplerState( int index );

		void ResizeSwapChain(int ID,UINT width,UINT height);
		void ResizeViewport(int ID,UINT width,UINT height);

		void QueueTask(TaskCore* pTask);
		void ProcessTaskQueue();


		int			CreateViewport(D3D11_VIEWPORT v);
		const Dx11ViewPort&			GetViewPort(int index);


		std::shared_ptr<Dx11Texture2D>		GetTexture2DByIndex(int ID);

		


		Microsoft::WRL::ComPtr<ID3D11Debug>			GetDebugDevice();

		TConfiguration<bool>						MultiThreadingConfig;

		void DeleteResource( ResourcePtr ptr );
		void DeleteResource( int index );
	protected:
		static ArkRenderer11* m_spRenderer;


		Microsoft::WRL::ComPtr<ID3D11Device>	m_pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;

		Microsoft::WRL::ComPtr<ID3D11Debug>				m_pDebugDevice;



		D3D_DRIVER_TYPE							m_driverType;

		std::vector<Dx11SwapChain*>				m_vSwapChains;

		std::vector<std::shared_ptr<Dx11Resource>>					m_vResources;

		std::vector<Dx11ViewPort>					m_vViewports;
		std::vector<RasterizerStateComPtr>			m_vRasterizerStates;

		std::vector<InputLayoutComPtr>				m_vInputLayouts;

		std::vector<Dx11ShaderResourceView>			m_vShaderResourceViews;
		std::vector<Dx11RenderTargetView>			m_vRenderTargetViews;
		std::vector<Dx11DepthStencilView>			m_vDepthStencilViews;
		std::vector<DepthStencilStateComPtr>		m_vDepthStencilStates;
		std::vector<BlendStateComPtr>				m_vBlendStates;
		std::vector<SamplerStateComPtr>				m_vSamplerStates;

		std::vector<ArkShader11SPtr>					m_vShaders;


		std::vector< TaskCore* >						m_vQueuedTasks;


		D3D_FEATURE_LEVEL			m_FeatureLevel;

	public:
		PipelineManager*								pPipeline;
		IParameterManager*								m_pParamMgr;
	public:
		std::shared_ptr<Dx11Resource>				GetResourceByIndex(int index);

		int							GetUnusedResourceIndex();
		int							StoreNewResource(std::shared_ptr<Dx11Resource> pResource);

		ResourcePtr					GetSwapChainResource(int ID);

	private:
			bool								m_bVsyncEnabled;
	};

};
unsigned int WINAPI _TaskThreadProc( void* lpParameter );

// Multithreading support objects
extern HANDLE						g_aThreadHandles[NUM_THREADS];
extern Arkeng::ThreadPayLoad		g_aPayload[NUM_THREADS];
extern HANDLE						g_aBeginEventHandle[NUM_THREADS];
extern HANDLE						g_aEndEventHandle[NUM_THREADS];

//--------------------------------------------------------------------------------
#endif
