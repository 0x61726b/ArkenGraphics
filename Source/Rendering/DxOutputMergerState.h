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
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class DxOutputMergerState
	{
	public:
		DxOutputMergerState();
		virtual ~DxOutputMergerState();

		void ClearState();

		void SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel );

		std::vector<int> RenderTargetViews;

		int			GetRenderTargetCount() const;
	protected:
		D3D_FEATURE_LEVEL				m_eFeatureLevel;

	};
};
//--------------------------------------------------------------------------------
#endif