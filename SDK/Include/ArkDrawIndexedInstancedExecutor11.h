//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkSpriteFont11.h
//--------------------------------------------------------------------------------
#ifndef __ArkDrawIndexedInstancedExecutor11_h__
#define __ArkDrawIndexedInstancedExecutor11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkDrawIndexedExecutor11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	template <class TVertex, class TInstance>
	class ArkDrawIndexedInstancedExecutor11 : public ArkDrawIndexedExecutor11<TVertex>
	{
	public:
		ArkDrawIndexedInstancedExecutor11( );
		virtual ~ArkDrawIndexedInstancedExecutor11( );
	
		virtual void Execute( PipelineManager* pPipeline, IParameterManager* pParamManager );
		virtual void ResetGeometry();
		virtual void ResetInstances();

		void AddInstance( const TInstance& data );
		unsigned int GetInstanceCount();

		void SetInstanceRange( unsigned int start, unsigned int end );

	protected:
		
		TGrowableVertexBuffer11<TInstance> InstanceBuffer;
		unsigned int m_uiStart;
		unsigned int m_uiCount;
	};

	#include "ArkDrawIndexedInstancedExecutor11.inl"
};
//--------------------------------------------------------------------------------
#endif // DrawIndexedInstancedExecutorDX11_h
//--------------------------------------------------------------------------------
