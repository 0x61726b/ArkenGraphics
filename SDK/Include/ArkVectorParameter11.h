//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVectorParameter11.h
//--------------------------------------------------------------------------------
#ifndef __ArkVectorParameter11_h__
#define __ArkVectorParameter11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkRenderParameter11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkVectorParameter11 : public ArkRenderParameter11
	{
	public:
		ArkVectorParameter11();
		ArkVectorParameter11(ArkVectorParameter11& copy);
		virtual ~ArkVectorParameter11();

		virtual void SetParameterData(void* pData,unsigned int thread = 0);

		virtual const ArkParamType GetParameterType();
		
		DirectX::XMVECTOR GetVector( unsigned int thread );
		void SetVector( DirectX::XMVECTOR v, unsigned int thread );

	protected:
		DirectX::XMVECTOR m_Vector[NUM_THREADS+1];
	};
};
//--------------------------------------------------------------------------------
#endif