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
namespace Arkeng
{
	class DxIAState
	{
	public:
		DxIAState();
		~DxIAState();

		void SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel );
		void Clear();

		unsigned int GetAvailableSlotCount();


		void SetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY );
		D3D_PRIMITIVE_TOPOLOGY GetPrimitiveTopology();
		//
		void AddVertexBuffer( int vb );
		void AddVertexBufferStride( unsigned int vbs );
		void AddVertexBufferOffsets( unsigned int vbos );
		std::vector<int>	GetVertexBuffers();
		std::vector<unsigned int>	GetStrides();
		std::vector<unsigned int>	GetOffsets();
		//

		//
		void SetIndexBuffer( int ib );
		void SetIndexBufferFormat( DXGI_FORMAT f );
		int GetIndexBuffer();
		DXGI_FORMAT GetIndexBufferFormat();
		//

		//
		void SetInputLayout( int il );
		int GetInputLayout();
		//
	protected:
		int								m_iIndexBuffer;
		DXGI_FORMAT						m_eIBFormat;
		std::vector<int>				m_vVertexBuffers;
		std::vector<unsigned int>		m_vVertexBufferStrides;
		std::vector<unsigned int>		m_vVertexBufferOffsets;
		int								m_vInputLayout;
		D3D_PRIMITIVE_TOPOLOGY			m_ePrimiviteTopology;

		unsigned int					m_uiAvailableSlotCount;

		D3D_FEATURE_LEVEL				m_FeatureLevel;
	};
};


//--------------------------------------------------------------------------------
#endif