//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxRasterizerState.h
//--------------------------------------------------------------------------------
#ifndef __DxRasterizerState_h__
#define __DxRasterizerState_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "TStateMonitor.h"
#include "TStateArrayMonitor.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class DxRasterizerState
	{
	public:
		DxRasterizerState();
		virtual ~DxRasterizerState();

		void SetFeatureLevel( D3D_FEATURE_LEVEL );
		void ClearState();

		void SetPreviousState( DxRasterizerState* pPrev );
		void ResetUpdateFlags();

		TStateMonitor<int>				RasterizerState;

		TStateMonitor<unsigned int>		ViewPortCount;
		TStateArrayMonitor<int, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE > ViewPorts;

		TStateMonitor< int > ScissorRectCount;
		TStateArrayMonitor< D3D11_RECT, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE > ScissorRects;

	protected:
		D3D_FEATURE_LEVEL		m_FeatureLevel;

		DxRasterizerState*		m_pPrevState;
	};
};
//--------------------------------------------------------------------------------
#endif