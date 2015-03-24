//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexBuffer11.h
//--------------------------------------------------------------------------------
#ifndef __ArkIndexBuffer11_h__
#define __ArkIndexBuffer11_h__
//--------------------------------------------------------------------------------
#include "ArkBuffer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkIndexBuffer11 : public ArkBuffer11
	{
	public:
		ArkIndexBuffer11( Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer );
		virtual ~ArkIndexBuffer11();

		virtual ResourceType				GetType();

		void						SetIndexSize( int size );
		void						SetIndexCount( int count );

	protected:
		int							m_iIndexSize;
		int							m_iIndexCount;

		friend ArkRenderer11;
	};
};
#endif
//--------------------------------------------------------------------------------
