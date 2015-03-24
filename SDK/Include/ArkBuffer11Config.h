//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkBuffer11Config.h
//--------------------------------------------------------------------------------
#ifndef __ArkBuffer11Config_h__
#define __ArkBuffer11Config_h__
//--------------------------------------------------------------------------------
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkBuffer11Config
	{
		public:
		ArkBuffer11Config();
		virtual ~ArkBuffer11Config();

		void SetDefaults();

		void SetDefaultConstantBuffer( UINT size, bool dynamic );
		void SetDefaultVertexBuffer( UINT size, bool dynamic );
		void SetDefaultIndexBuffer( UINT size, bool dynamic );
		void SetDefaultStructuredBuffer( UINT size, UINT structsize );
		void SetDefaultByteAddressBuffer( UINT size );
		void SetDefaultIndirectArgsBuffer( UINT size );
		void SetDefaultStagingBuffer( UINT size );

		void SetByteWidth( UINT state );
		void SetUsage( D3D11_USAGE state );
	    void SetBindFlags( UINT state );
	    void SetCPUAccessFlags( UINT state );
	    void SetMiscFlags( UINT state );	
	    void SetStructureByteStride( UINT state );

		D3D11_BUFFER_DESC GetBufferDesc();

	protected:
		D3D11_BUFFER_DESC 		m_State;

		friend ArkRenderer11;
	};
};
//--------------------------------------------------------------------------------
#endif //
