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
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class RendererDX11 : public IRenderer
	{
	public:
		RendererDX11();
		virtual ~RendererDX11();

		static RendererDX11* Get();

		virtual bool Initialize( D3D_DRIVER_TYPE DriverType, D3D_FEATURE_LEVEL FeatureLevel );
		virtual void Shutdown();

		virtual void Present( HWND hWnd = 0,int swapchain = -1 );

	protected:
		static RendererDX11* m_spRenderer;
	};
};

//--------------------------------------------------------------------------------
#endif
