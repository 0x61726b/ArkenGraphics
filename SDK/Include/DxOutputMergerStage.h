//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxOutputMergerStage.h
//--------------------------------------------------------------------------------
#ifndef __DxOutputMergerStage_h__
#define __DxOutputMergerStage_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11ResourceProxy.h"
#include "DxOutputMergerState.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class PipelineManager;

	class DxOutputMergerStage
	{
	public:
		DxOutputMergerStage();
		virtual ~DxOutputMergerStage();

		void ClearState();

		void ApplyRenderTargets( ID3D11DeviceContext* Context );
		
		void SetFeatureLevel( D3D_FEATURE_LEVEL Fl );

		const DxOutputMergerState& GetCurrentState() const;

		DxOutputMergerState			State;

	protected:
		D3D_FEATURE_LEVEL			m_eFeatureLevel;


		friend PipelineManager;
	};
};

//--------------------------------------------------------------------------------
#endif