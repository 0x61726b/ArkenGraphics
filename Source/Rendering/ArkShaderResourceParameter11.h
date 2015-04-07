//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkShaderResourceParameter11.h
//--------------------------------------------------------------------------------
#ifndef __ArkShaderResourceParameter11_h__
#define __ArkShaderResourceParameter11_h__
//--------------------------------------------------------------------------------
#include "ArkRenderParameter11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkShaderResourceParameter11 : public ArkRenderParameter11
	{
	public:
		ArkShaderResourceParameter11();
		ArkShaderResourceParameter11( ArkShaderResourceParameter11& copy );
		virtual ~ArkShaderResourceParameter11();

		virtual void SetParameterData( void* pData, unsigned int threadID = 0 );

		virtual const ArkParamType GetParameterType();
		int GetIndex( unsigned int threadID );

	protected:
		int		m_iShaderResourceView[NUM_THREADS+1];
	};
};
//--------------------------------------------------------------------------------
#endif