//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11RenderTargetView.h
//--------------------------------------------------------------------------------
#ifndef __Dx11ShaderResourceView__
#define __Dx11ShaderResourceView__
//--------------------------------------------------------------------------------
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11ShaderResourceView
	{
	public:
		Dx11ShaderResourceView( ShaderResourceViewComPtr );
		~Dx11ShaderResourceView();

		ID3D11ShaderResourceView* GetSRV();

	protected:
		ShaderResourceViewComPtr			m_pShaderResourceView;

		friend PipelineManager;
		friend D3D11Renderer;
	};
};


//--------------------------------------------------------------------------------
#endif