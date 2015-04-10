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
#include "ArkRenderer11.h"
#include "RenderTask.h"
#include "Camera.h"
#include "ViewTextOverlay.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkRenderApplication11 : public ArkApplication
	{
	public:
		ArkRenderApplication11();
		virtual ~ArkRenderApplication11();

	public:
		virtual bool ConfigureRenderingEngineComponents(UINT width,UINT height,D3D_FEATURE_LEVEL desiredLevel,D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE);
		virtual bool ConfigureRenderingSetup();

		virtual void ShutdownRenderingEngineComponents();
		virtual void ShutdownRenderingSetup();

		virtual void HandleWindowResize(HWND handle,UINT width,UINT height);
		virtual bool HandleEvent(EventPtr pEvent);

		void SetMultiThreadedMode( bool mode );
		virtual Win32RenderWindow* CreateRenderWindow();

	protected:
		ArkRenderer11*			m_pRenderer;
		Win32RenderWindow*		m_pWindow;

		RenderTask*				m_pRenderView;
		ViewTextOverlay*		m_pTextOverlayView;

		Camera*					m_pCamera;

		ResourcePtr				m_pBackBuffer;

		EventManager			CameraEventHub;

		UINT					m_iWidth;
		UINT					m_iHeight;

		
	};

};


//--------------------------------------------------------------------------------
#endif