//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ResourceProxy.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11ResourceProxy.h"
#include "Dx11Texture2DConfig.h"
#include "ArkBuffer11Config.h"
#include "Dx11DepthStencilViewConfig.h"
#include "Dx11ShaderResourceViewConfig.h"
#include "Dx11RenderTargetViewConfig.h"
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11ResourceProxy::Dx11ResourceProxy()
{
	m_iResource = -1;

	m_iResourceDSV = m_iResourceRTV = m_iResourceSRV = 0;

	m_pBufferConfig = 0;
	m_pTexture2dConfig = nullptr;
	m_pSRVConfig = nullptr;
	m_pRTVConfig = nullptr;
	m_pDSVConfig = nullptr;
}
//--------------------------------------------------------------------------------
Dx11ResourceProxy::~Dx11ResourceProxy()
{
	Safe_Delete(m_pBufferConfig);
	Safe_Delete(m_pTexture2dConfig);
	Safe_Delete(m_pSRVConfig);
	Safe_Delete(m_pSRVConfig);
	Safe_Delete(m_pDSVConfig);
}
//--------------------------------------------------------------------------------
Dx11ResourceProxy::Dx11ResourceProxy(int ResourceID,Dx11Texture2DConfig* pConfig,ArkRenderer11* pRenderer,Dx11ShaderResourceViewConfig* pSRVConfig,
	Dx11RenderTargetViewConfig* pRTVConfig,
	Dx11DepthStencilViewConfig* pDSVConfig)
{
	D3D11_TEXTURE2D_DESC desc = pConfig->GetTextureDesc();
	CommonConstructor(desc.BindFlags,ResourceID,pRenderer,pSRVConfig,pRTVConfig,pDSVConfig);

	m_pTexture2dConfig = new Dx11Texture2DConfig();
	*m_pTexture2dConfig = *pConfig;
}
//--------------------------------------------------------------------------------
Dx11ResourceProxy::Dx11ResourceProxy(int ResourceID,ArkBuffer11Config* pConfig,ArkRenderer11* pRenderer,Dx11ShaderResourceViewConfig* pSRVConfig,
	Dx11RenderTargetViewConfig* pRTVConfig,
	Dx11DepthStencilViewConfig* pDSVConfig)
{
	D3D11_BUFFER_DESC desc = pConfig->GetBufferDesc();
	CommonConstructor(desc.BindFlags,ResourceID,pRenderer,pSRVConfig,pRTVConfig,pDSVConfig);

	m_pBufferConfig = new ArkBuffer11Config();
	*m_pBufferConfig = *pConfig;
}
//--------------------------------------------------------------------------------
void Dx11ResourceProxy::CommonConstructor(UINT BindFlags,int ResourceID,ArkRenderer11* pRenderer,
	Dx11ShaderResourceViewConfig* pSRVConfig,
	Dx11RenderTargetViewConfig* pRTVConfig,
	Dx11DepthStencilViewConfig* pDSVConfig)
{
	m_iResource = ResourceID;
	m_iResourceSRV = 0;
	m_iResourceRTV = 0;
	m_iResourceDSV = 0;

	m_pTexture2dConfig = nullptr;
	m_pBufferConfig = nullptr;
	m_pRTVConfig = nullptr;
	m_pDSVConfig = nullptr;
	m_pSRVConfig = nullptr;

	if(pSRVConfig)
	{
		m_pSRVConfig = new Dx11ShaderResourceViewConfig();
		*m_pSRVConfig = *pSRVConfig;
	}

	if(pRTVConfig)
	{
		m_pRTVConfig = new Dx11RenderTargetViewConfig();
		*m_pRTVConfig = *pRTVConfig;
	}
	if(pDSVConfig)
	{
		m_pDSVConfig = new Dx11DepthStencilViewConfig();
		*m_pDSVConfig = *pDSVConfig;
	}


	//Add suitable methods to the renderer to create default views
	if((BindFlags & D3D11_BIND_RENDER_TARGET) == D3D11_BIND_RENDER_TARGET)
	{
		D3D11_RENDER_TARGET_VIEW_DESC* pDesc = pRTVConfig ? &pRTVConfig->GetRTVDesc() : nullptr;
		m_iResourceRTV = pRenderer->CreateRenderTargetView(m_iResource,pDesc);
	}
	if((BindFlags & D3D11_BIND_DEPTH_STENCIL) == D3D11_BIND_DEPTH_STENCIL)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc = pDSVConfig ? &pDSVConfig->GetDSVDesc() : nullptr;
		m_iResourceDSV = pRenderer->CreateDepthStencilView(m_iResource,pDesc);
	}
}