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
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class DxRasterizerState
	{
	public:
		DxRasterizerState();
		virtual ~DxRasterizerState();

		void SetFeatureLevel( D3D_FEATURE_LEVEL );
		void Clear();

		void AddViewport( int viewport );
		void SetViewportCount( unsigned int count );
		int  GetViewportCount();
	public:
		std::vector<int>		m_vViewports;
		unsigned int			m_iViewportCount;

	protected:
		D3D_FEATURE_LEVEL		m_FeatureLevel;

	};
};
//--------------------------------------------------------------------------------
#endif