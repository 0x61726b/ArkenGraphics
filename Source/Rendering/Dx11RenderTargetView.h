//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11RenderTargetView.h
//--------------------------------------------------------------------------------
#ifndef __Dx11RenderTargetView_h__
#define __Dx11RenderTargetView_h__
//--------------------------------------------------------------------------------
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11RenderTargetView
	{
	public:
		Dx11RenderTargetView( RenderTargetViewComPtr );
		~Dx11RenderTargetView();

		ID3D11RenderTargetView* GetRTV();

	protected:
		RenderTargetViewComPtr			m_pRenderTargetView;

		friend PipelineManager;
		friend DxOutputMergerStage;
		friend D3D11Renderer;

	};
};


//--------------------------------------------------------------------------------
#endif