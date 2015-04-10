//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkDrawExecutor11.h
//--------------------------------------------------------------------------------
#ifndef __ArkDrawIndexedExecutor11_h__
#define __ArkDrawIndexedExecutor11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkDrawExecutor11.h"
#include "TGrowableIndexBuffer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	template <class TVertex>
	class ArkDrawIndexedExecutor11 : public ArkDrawExecutor11<TVertex>
	{
	public:
		ArkDrawIndexedExecutor11( );
		virtual ~ArkDrawIndexedExecutor11( );
	
		virtual void Execute( PipelineManager* pPipeline, IParameterManager* pParamManager );
		virtual void ResetGeometry();
		virtual void ResetIndices();

		void AddIndex( const unsigned int index );
		void AddIndices( const unsigned int i1, const unsigned int i2 );
		void AddIndices( const unsigned int i1, const unsigned int i2, const unsigned int i3 );

		unsigned int GetIndexCount();
		virtual unsigned int GetPrimitiveCount();

		void SetMaxIndexCount( unsigned int count );
	protected:
		
		TGrowableIndexBuffer11<unsigned int> IndexBuffer;
	};
	#include "ArkDrawIndexedExecutor11.inl"
};
//--------------------------------------------------------------------------------
#endif