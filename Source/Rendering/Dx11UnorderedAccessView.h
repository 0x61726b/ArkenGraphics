//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11RenderTargetView.h
//--------------------------------------------------------------------------------
#ifndef __Dx11UnorderedAccessView__
#define __Dx11UnorderedAccessView__
//--------------------------------------------------------------------------------
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11UnorderedAccessView
	{
	public:
		Dx11UnorderedAccessView( UnorderedAccessViewComPtr );
		~Dx11UnorderedAccessView();

	protected:
		UnorderedAccessViewComPtr			m_pUnorderedAccessView;

		friend DxOutputMergerStage;
		friend PipelineManager;
		friend ArkRenderer11;
	};
};


//--------------------------------------------------------------------------------
#endif