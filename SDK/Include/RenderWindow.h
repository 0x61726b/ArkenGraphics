//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RenderWindow.h
//--------------------------------------------------------------------------------
#ifndef __RenderWindow_h__
#define __RenderWindow_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "IWndProc.h"
namespace Arkeng
{
	class RenderWindow
	{
	public:
		RenderWindow();
		virtual ~RenderWindow();

		virtual void Initialize(IWndProc* WindowProcObj) = 0;
		virtual void Shutdown() = 0;
		virtual void Paint() = 0;

		HWND GetHandle();

		void SetWidth(int width);
		void SetHeight(int height);

		int GetWidth();
		int GetHeight();
		int GetLeft();
		int GetTop();

		void SetSize(int width,int height);
		void SetPosition(int left,int top);

		void ResizeWindow(int width,int height);

		int GetSwapChain();
		void SetSwapChain(int swapchain);

		void SetCaption(std::wstring& caption);
		std::wstring GetCaption();

		void SetStyle(DWORD dStyle);
		DWORD GetStyle();

	protected:

		void UpdateWindowState();

		HWND			m_hWnd;
		std::wstring	m_sCaption;
		int				m_iWidth;
		int				m_iHeight;
		int				m_iLeft;
		int				m_iTop;

		int				m_iSwapChain;

		DWORD			m_dStyle;


	};

};








//--------------------------------------------------------------------------------
#endif