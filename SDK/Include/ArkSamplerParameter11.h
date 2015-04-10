//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkParameterManager11.h
//--------------------------------------------------------------------------------
#ifndef __ArkSamplerParameter11_h__
#define __ArkSamplerParameter11_h__
//--------------------------------------------------------------------------------
#include "ArkRenderParameter11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkSamplerParameter11 : public ArkRenderParameter11
	{
	public:
		ArkSamplerParameter11();
		ArkSamplerParameter11( ArkSamplerParameter11& copy );
		virtual ~ArkSamplerParameter11();

		virtual void SetParameterData( void* pData, unsigned int threadID = 0 );
		//virtual void ResetParameterData( void* pData, unsigned int threadID = 0 );

		virtual const ArkParamType GetParameterType();
		int GetIndex( unsigned int threadID );

		//void UpdateValue( RenderParameterDX11* pParameter, unsigned int threadID = 0 );

	protected:
		int		m_iSampler[NUM_THREADS+1];
	};
};
//--------------------------------------------------------------------------------
#endif // SamplerParameterDX11_h
//--------------------------------------------------------------------------------

