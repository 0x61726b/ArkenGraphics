//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxOutputMergerState.h
//--------------------------------------------------------------------------------
#ifndef __DxOutputMergerState_h__
#define __DxOutputMergerState_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "TStateMonitor.h"
#include "TStateArrayMonitor.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class DxOutputMergerState
	{
	public:
		DxOutputMergerState();
		virtual ~DxOutputMergerState();

		void ClearState();
		void ResetUpdate();
		void SetPreviosState( DxOutputMergerState* pPrev );

		int GetRenderTargetCount() const;

		void SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel );

		TStateMonitor<int>													BlendState;
		TStateMonitor<int>													DepthStencilState;
		TStateMonitor<unsigned int>											StencilRef;

		TStateMonitor<int>													DepthTargetViews;
		TStateArrayMonitor<int,D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT>		RenderTargetViews;
		TStateArrayMonitor< int, D3D11_PS_CS_UAV_REGISTER_COUNT >			UnorderedAccessViews;
		TStateArrayMonitor< unsigned int, D3D11_PS_CS_UAV_REGISTER_COUNT >  UAVInitialCounts;

	protected:
		D3D_FEATURE_LEVEL				m_eFeatureLevel;

		DxOutputMergerState*			m_pPrevState;


	};
};
//--------------------------------------------------------------------------------
#endif