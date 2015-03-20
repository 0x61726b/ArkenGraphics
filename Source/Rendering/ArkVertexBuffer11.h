//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexBuffer11.h
//--------------------------------------------------------------------------------
#ifndef __ArkVertexBuffer11_h__
#define __ArkVertexBuffer11_h__
//--------------------------------------------------------------------------------
#include "ArkBuffer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkVertexBuffer11 : public ArkBuffer11
	{
	public:
		ArkVertexBuffer11( Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer );
		virtual ~ArkVertexBuffer11();

		virtual ResourceType				GetType();

		void						SetVertexSize( int size );
		void						SetVertexCount( int count );

	protected:
		int							m_iVertexSize;
		int							m_iVertexCount;

		friend ArkRenderer11;
	};
};
#endif
//--------------------------------------------------------------------------------
