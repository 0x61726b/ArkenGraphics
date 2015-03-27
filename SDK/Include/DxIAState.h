//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxIAState.h
//--------------------------------------------------------------------------------
#ifndef __DxIAState_h__
#define __DxIAState_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "TStateMonitor.h"
#include "TStateArrayMonitor.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class DxIAState
	{
	public:
		DxIAState();
		~DxIAState();

		void SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel );
		void ClearState();

		unsigned int GetAvailableSlotCount();

		void SetPreviousState( DxIAState* p );
		void ResetUpdate();


		TStateMonitor< int > InputLayout;
		TStateMonitor< D3D_PRIMITIVE_TOPOLOGY > PrimitiveTopology;
		TStateArrayMonitor< int,D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT >	VertexBuffers;
		TStateArrayMonitor< unsigned int, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT > VertexBufferStrides;
		TStateArrayMonitor< unsigned int, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT > VertexBufferOffsets;

		TStateMonitor< int>			 IndexBuffer;
		TStateMonitor< DXGI_FORMAT > IndexBufferFormat;
	protected:

		unsigned int					m_uiAvailableSlotCount;

		DxIAState*						m_pPreviousState;

		D3D_FEATURE_LEVEL				m_FeatureLevel;
	};
};


//--------------------------------------------------------------------------------
#endif