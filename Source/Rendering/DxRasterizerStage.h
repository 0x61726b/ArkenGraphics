//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxRasterizerStage.h
//--------------------------------------------------------------------------------
#ifndef __DxRasterizerStage_h__
#define __DxRasterizerStage_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11ResourceProxy.h"
#include "DxRasterizerState.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class PipelineManager;

	class DxRasterizerStage
	{
	public:
		DxRasterizerStage();
		virtual ~DxRasterizerStage();

		void ClearState();
		
		void SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel );

		const DxRasterizerState& GetCurrentState() const;

		void ApplyState( ID3D11DeviceContext* pContext );

	public:
		DxRasterizerState		State;

	protected:
		D3D_FEATURE_LEVEL m_FeatureLevel;

		friend PipelineManager;

	};
};
//--------------------------------------------------------------------------------
#endif