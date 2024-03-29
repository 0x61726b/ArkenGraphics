//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkRenderParameter11.h
//--------------------------------------------------------------------------------
#ifndef __ArkRenderParameter11_h__
#define __ArkRenderParameter11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	enum ArkParamType
	{
		VECTOR,
		MATRIX,
		MATRIX_ARRAY,
		SHADER_RESOURCE,
		UOA,
		CBUFFER,
		SAMPLER,
		ENTITY
	};
	class ArkRenderParameter11
	{
	public:
		ArkRenderParameter11();
		ArkRenderParameter11(ArkRenderParameter11& copy);
		~ArkRenderParameter11();

		void SetName( const std::wstring& name );
		std::wstring& GetName();

		virtual const ArkParamType GetParameterType() = 0;

		void InitializeParameterData( void* pData );

		virtual void SetParameterData( void* pData,unsigned int thread=0 ) = 0;

		unsigned int GetValueID( unsigned int threadID = 0 );
	protected:
		std::wstring		m_sParameterName;
		unsigned int	m_auiValueID[NUM_THREADS+1];

	};
}
//--------------------------------------------------------------------------------
#endif