//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PipelineManager.h
//--------------------------------------------------------------------------------
#ifndef __PipelineManager_h__
#define __PipelineManager_h__
//--------------------------------------------------------------------------------
#include "Dx11ResourceProxy.h"
#include "Dx11Resource.h"
#include "DxOutputMergerStage.h"
#include "DxRasterizerStage.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class PipelineManager
	{
	public:
		PipelineManager();
		~PipelineManager();

		void SetDeviceContext(DeviceContextComPtr Context,D3D_FEATURE_LEVEL FeatureLevel);

		void ClearRenderTargets();
		void ApplyRenderTargets();

		void ApplyPipelineResources();
		void ClearPipelineResources();

		void ClearPipelineState();

		void ClearBuffers( float color[],float depth );

		D3D_FEATURE_LEVEL						m_FeatureLevel;

		DeviceContextComPtr			            m_pContext;

		DxOutputMergerStage						OutputMergerStage;
		DxRasterizerStage						RasterizerStage;
	};
};
//--------------------------------------------------------------------------------
#endif