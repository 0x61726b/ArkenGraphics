//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11DepthStencilView.h
//--------------------------------------------------------------------------------
#ifndef __Dx11DepthStencilView_h__
#define __Dx11DepthStencilView_h__
//--------------------------------------------------------------------------------
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11DepthStencilView
	{
	public:
		Dx11DepthStencilView( DepthStencilViewComPtr );
		~Dx11DepthStencilView();

	protected:
		DepthStencilViewComPtr			m_pDepthStencilView;

		friend PipelineManager;
		friend ArkRenderer11;
	};
};


//--------------------------------------------------------------------------------
#endif