//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxIAStage.h
//--------------------------------------------------------------------------------
#ifndef __DxIAStage_h__
#define __DxIAStage_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "DxIAState.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class DxIAStage
	{
	public:
		DxIAStage();
		virtual ~DxIAStage();

		void SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel );
		void ClearState();
		void ApplyState( ID3D11DeviceContext* pContext );

		const DxIAState& GetState() const;

		DxIAState				CurrentState;
	protected:
		D3D_FEATURE_LEVEL		FeatureLevel;
		
	};
};
//--------------------------------------------------------------------------------
#endif