//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxStreamOutputStageState11.h
//--------------------------------------------------------------------------------
#ifndef __Dx11StreamOutputStage11_h__
#define __Dx11StreamOutputStage11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11ResourceProxy.h"
#include "Dx11StreamOutputStageState.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class PipelineManager;

	class Dx11StreamOutputStage
	{
	public:
		Dx11StreamOutputStage();
		virtual ~Dx11StreamOutputStage();

		void SetFeautureLevel( D3D_FEATURE_LEVEL level );

		void ClearCurrentState( );
		void ClearPreviousState( );
		void ApplyDesiredState( ID3D11DeviceContext* pContext );

		const Dx11StreamOutputStageState& GetPreviousState() const;

		Dx11StreamOutputStageState	CurrentState;

	protected:

		D3D_FEATURE_LEVEL				m_FeatureLevel;

		Dx11StreamOutputStageState		PreviousState;

		friend PipelineManager;
	};
};
//--------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------

