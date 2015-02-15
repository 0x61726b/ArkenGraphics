//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Pch.h
//--------------------------------------------------------------------------------
#ifndef _IRenderer_h__
#define _IRenderer_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	enum RendererType
	{
		Direct3D11
	};

	class IRenderer
	{
	public:
		virtual ~IRenderer() {}

		virtual bool Initialize(int w,int h,bool windowed,HWND handle) { return false; };
		virtual void Shutdown() = 0;
		virtual void Present(HWND hWnd = 0,int SwapChain = -1) = 0;

	protected:
		IRenderer& operator=(const IRenderer&);
	};
};
//--------------------------------------------------------------------------------
#endif