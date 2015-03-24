//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkConstantBufferParameter11.h
//--------------------------------------------------------------------------------
#ifndef __ArkConstantBufferParameter11_h__
#define __ArkConstantBufferParameter11_h__
//--------------------------------------------------------------------------------
#include "ArkRenderParameter11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkConstantBufferParameter11 : public ArkRenderParameter11
	{
	public:

		ArkConstantBufferParameter11();
		ArkConstantBufferParameter11(ArkConstantBufferParameter11& copy);
		virtual ~ArkConstantBufferParameter11();

		virtual void SetParameterData(void* pData,unsigned int threadID = 0);
		//virtual void ResetParameterData( void* pData, unsigned int threadID = 0 );

		virtual const ArkParamType GetParameterType();

		//virtual void UpdateValue( RenderParameterDX11* pParameter, unsigned int threadID = 0 );

		int GetIndex(unsigned int threadID);

	protected:
		int		m_iCBuffer[NUM_THREADS+1];
	};

};

//--------------------------------------------------------------------------------
#endif