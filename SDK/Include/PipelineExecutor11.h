//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//X.h
//--------------------------------------------------------------------------------
#ifndef __PipelineExecutor11_h__
#define __PipelineExecutor11_h__
//--------------------------------------------------------------------------------
#include "PCH.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class PipelineManager;
	class IParameterManager;

	// This simple structure provides the mapping between a vertex shader program
	// and the corresponding input layout.

	struct InputLayoutKey
	{
		int shader;
		int layout;
	};

	class PipelineExecutor11
	{
	public:
		PipelineExecutor11();
		virtual ~PipelineExecutor11();

		virtual void Execute( PipelineManager* pPipeline, IParameterManager* pParamManager ) = 0;
		
		virtual void SetLayoutElements( unsigned int count, D3D11_INPUT_ELEMENT_DESC* pElements );
		virtual void GenerateInputLayout( int ShaderID );
		virtual int GetInputLayout( int ShaderID );

	protected:

		// A description of our vertex elements
		std::vector<D3D11_INPUT_ELEMENT_DESC>	m_elements;
		std::map<int,InputLayoutKey*>			m_InputLayouts;

	};

	typedef std::shared_ptr<PipelineExecutor11> ExecutorPtr;
};
//--------------------------------------------------------------------------------
#endif // PipelineExecutorDX11_h
//--------------------------------------------------------------------------------

