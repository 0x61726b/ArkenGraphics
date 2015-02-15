//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ResourceProxy.h
//--------------------------------------------------------------------------------
#ifndef __DX11Texture2DConfig_h__
#define __DX11Texture2DConfig_h__
//--------------------------------------------------------------------------------
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11Texture2DConfig
	{
	public:
		Dx11Texture2DConfig();
		virtual ~Dx11Texture2DConfig();

		void SetDefaults();
		void SetDepthBuffer( UINT width, UINT height );
		void SetColorBuffer( UINT width, UINT height );

		void SetWidth( UINT state );
		void SetHeight( UINT state );
		void SetMipLevels( UINT state );
		void SetArraySize( UINT state );
		void SetFormat( DXGI_FORMAT state );
		void SetSampleDesc( DXGI_SAMPLE_DESC state );
		void SetUsage( D3D11_USAGE state ); 
		void SetBindFlags( UINT state );
		void SetCPUAccessFlags( UINT state );
		void SetMiscFlags( UINT state );

		D3D11_TEXTURE2D_DESC GetTextureDesc();

	protected:
		D3D11_TEXTURE2D_DESC 		m_State;

		friend ArkRenderer11;
	};
};
#endif