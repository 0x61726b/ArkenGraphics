//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//D3D11RenderApplication.h
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#ifndef __D3D11RenderApplication_h__
#define __D3D11RenderApplication_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkApplication.h"
#include "Win32RenderWindow.h"
#include "D3D11Renderer.h"
#include "RenderTask.h"
#include "Camera.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class D3D11RenderApplication : public ArkApplication
	{
	public:
		D3D11RenderApplication();
		virtual ~D3D11RenderApplication();

	public:
		virtual bool ConfigureRenderingEngineComponents(UINT width,UINT height,D3D_FEATURE_LEVEL desiredLevel,D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE);
		virtual bool ConfigureRenderingSetup();

		virtual void ShutdownRenderingEngineComponents();
		virtual void ShutdownRenderingSetup();

		virtual void HandleWindowResize(HWND handle,UINT width,UINT height);
		virtual bool HandleEvent(EventPtr pEvent);

		virtual Win32RenderWindow* CreateRenderWindow();

	protected:
		D3D11Renderer*			m_pRenderer;
		Win32RenderWindow*		m_pWindow;
		RenderTask*				m_pRenderView;

		Camera*					m_pCamera;

		ResourcePtr						m_pBackBuffer;
		ResourcePtr							m_testBackBuffer;

		UINT					m_iWidth;
		UINT					m_iHeight;
	};

};


//--------------------------------------------------------------------------------
#endif