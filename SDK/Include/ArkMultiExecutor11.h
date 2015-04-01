//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkMultiExecutor11.h
//--------------------------------------------------------------------------------
#ifndef __ArkMultiExecutor11_h__
#define __ArkMultiExecutor11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "PipelineExecutor11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class PipelineManager;
	class IParameterManager;

	class ArkMultiExecutor11 : public PipelineExecutor11
	{
	public:
		ArkMultiExecutor11();
		virtual ~ArkMultiExecutor11();

		virtual void Execute( PipelineManager* pPipeline, IParameterManager* pParamManager );
		
		virtual void SetLayoutElements( unsigned int count, D3D11_INPUT_ELEMENT_DESC* pElements );
		virtual void GenerateInputLayout( int ShaderID );
		virtual int GetInputLayout( int ShaderID );

		void AddExecutor( ExecutorPtr pExecutor );

	protected:
		std::vector<ExecutorPtr> m_Executors;
	};

	typedef std::shared_ptr<ArkMultiExecutor11> MultiExecutorPtr;
};
//--------------------------------------------------------------------------------
#endif // MultiExecutorDX11_h
//--------------------------------------------------------------------------------

