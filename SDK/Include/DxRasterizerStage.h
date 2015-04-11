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

		void ClearCurrentState();
		void ClearPreviousState();
		void ApplyCurrentState( ID3D11DeviceContext* pContext );


		void SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel );
		const DxRasterizerState& GetPreviousState() const;
	public:
		DxRasterizerState		CurrentState;

	protected:
		D3D_FEATURE_LEVEL m_FeatureLevel;

		DxRasterizerState		PreviousState;

		friend PipelineManager;

	};
};
//--------------------------------------------------------------------------------
#endif