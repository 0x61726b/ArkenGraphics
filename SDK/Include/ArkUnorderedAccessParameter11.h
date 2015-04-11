//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// UnorderedAccessParameterDX11
//
//--------------------------------------------------------------------------------
#ifndef __ArkUnorderedAccessParameter11_h__
#define __ArkUnorderedAccessParameter11_h__
//--------------------------------------------------------------------------------
#include "ArkRenderParameter11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	struct UAVParameterData
	{
		int				m_iUnorderedAccessView;
		unsigned int	m_iInitialCount;
	};

	class ArkUnorderedAccessParameter11 : public ArkRenderParameter11
	{
	public:
		ArkUnorderedAccessParameter11();
		ArkUnorderedAccessParameter11( ArkUnorderedAccessParameter11& copy );
		virtual ~ArkUnorderedAccessParameter11();

		virtual void SetParameterData( void* pData, unsigned int threadID = 0 );
		//virtual void ResetParameterData( void* pData, unsigned int threadID = 0 );

		virtual const ArkParamType GetParameterType();
		int GetIndex( unsigned int threadID );
		unsigned int GetInitialCount( unsigned int threadID );

		//void UpdateValue( RenderParameterDX11* pParameter, unsigned int threadID = 0 );

	protected:
		UAVParameterData m_ParameterData[NUM_THREADS+1];
	};
};
//--------------------------------------------------------------------------------
#endif // UnorderedAccessParameterDX11_h
//--------------------------------------------------------------------------------

