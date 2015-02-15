//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RendererDX11.h
//--------------------------------------------------------------------------------
#include "D3D11Renderer.h"
//--------------------------------------------------------------------------------
#ifndef __ResourceDX11_h__
#define __ResourceDX11_h__
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ResourceD3D11 
	{
	public:
		ResourceD3D11();
		virtual ~ResourceD3D11();

		virtual ResourceType	GetType() = 0;
		virtual ID3D11Resource* GetResource() = 0;

		virtual UINT						GetEvictionPriority() = 0;
		virtual void						SetEvictionPriority( UINT EvictionPriority ) = 0;

		unsigned short						GetInnerID();

		static unsigned short				s_usResourceUID;
		unsigned short						m_usInnerID;
	};
};

//--------------------------------------------------------------------------------
#endif