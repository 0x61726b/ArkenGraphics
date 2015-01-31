//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ResourceProxy.h
//--------------------------------------------------------------------------------
#ifndef __Texture2D_h__
#define __Texture2D_h__
//--------------------------------------------------------------------------------
#include "Dx11Resource.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11Texture2D : public Dx11Resource
	{
		explicit Dx11Texture2D( Microsoft::WRL::ComPtr<ID3D11Texture2D> pTex );
		virtual ~Dx11Texture2D();

		D3D11_TEXTURE2D_DESC		GetActualDescription();
		D3D11_TEXTURE2D_DESC		GetDesiredDescription();
		void						SetDesiredDescription( D3D11_TEXTURE2D_DESC description );

		void						CreateTexture( void* pData );

		virtual ResourceType				GetType();
		virtual ID3D11Resource*				GetResource();

		virtual UINT						GetEvictionPriority();
		virtual void						SetEvictionPriority( UINT EvictionPriority );

	protected:
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_pTexture;
		D3D11_TEXTURE2D_DESC							m_DesiredDesc;
		D3D11_TEXTURE2D_DESC							m_ActualDesc;

		friend D3D11Renderer;
	};
};
//--------------------------------------------------------------------------------
#endif
