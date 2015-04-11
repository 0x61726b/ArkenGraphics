//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxStreamOutputStageState11.h
//--------------------------------------------------------------------------------
#ifndef __DxStreamOutputStageState11_h__
#define __DxStreamOutputStageState11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "TStateMonitor.h"
#include "TStateArrayMonitor.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class PipelineManager;

	class Dx11StreamOutputStageState
	{
	public:
		Dx11StreamOutputStageState();
		virtual ~Dx11StreamOutputStageState();

		void SetFeautureLevel( D3D_FEATURE_LEVEL level );
		void ClearState( );
		void SetSisterState( Dx11StreamOutputStageState* pState );
		void ResetUpdateFlags( );

		int GetBufferCount() const;

		TStateArrayMonitor< int, 4 >  StreamBuffers;
		TStateArrayMonitor< unsigned int, 4 > StreamOffsets;

	protected:

		D3D_FEATURE_LEVEL				m_FeatureLevel;

		Dx11StreamOutputStageState*		m_pSisterState;
	};
};
//--------------------------------------------------------------------------------
#endif // DxStreamOutputStageState11
//--------------------------------------------------------------------------------

