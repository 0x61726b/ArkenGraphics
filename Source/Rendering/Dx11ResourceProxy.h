//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ResourceProxy.h
//--------------------------------------------------------------------------------
#ifndef __Dx11ResourceProxy_h__
#define __Dx11ResourceProxy_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11Texture2DConfig;
	class Dx11ShaderResourceViewConfig;
	class Dx11RenderTargetViewConfig;
	class Dx11DepthStencilViewConfig;
	class D3D11Renderer;

	class Dx11ResourceProxy
	{
	public:
		Dx11ResourceProxy();

		Dx11ResourceProxy(int ResourceID,Dx11Texture2DConfig* pConfig,D3D11Renderer* pRenderer,
			Dx11ShaderResourceViewConfig* pSRVConfig = NULL,
			Dx11RenderTargetViewConfig* pRTVConfig = NULL,
			Dx11DepthStencilViewConfig* pDSVConfig = NULL);

		virtual ~Dx11ResourceProxy();

	public:
		int								m_iResource;
		int								m_iResourceSRV;
		int								m_iResourceRTV;
		int								m_iResourceDSV;

		Dx11Texture2DConfig*	        m_pTexture2dConfig;
		Dx11ShaderResourceViewConfig*   m_pSRVConfig;
		Dx11RenderTargetViewConfig*     m_pRTVConfig;
		Dx11DepthStencilViewConfig*		m_pDSVConfig;

		friend D3D11Renderer;
	protected:
		void CommonConstructor(UINT BindFlags,int ResourceID,D3D11Renderer* pRenderer,
			Dx11ShaderResourceViewConfig* pSRVConfig,
			Dx11RenderTargetViewConfig* pRTVConfig,
			Dx11DepthStencilViewConfig* pDSVConfig);
	};
	typedef std::shared_ptr<Dx11ResourceProxy> ResourcePtr;
};

//--------------------------------------------------------------------------------
#endif
