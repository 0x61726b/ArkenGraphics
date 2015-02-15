//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RendererDX11.h
//--------------------------------------------------------------------------------
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
#ifndef __Dx11Resource_h__
#define __Dx11Resource_h__
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11Resource
	{
	public:
		Dx11Resource();
		virtual ~Dx11Resource();

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