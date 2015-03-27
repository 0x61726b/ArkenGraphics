//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkRenderEffect11.h
//--------------------------------------------------------------------------------
#ifndef __ArkRenderEffect11_h__
#define __ArkRenderEffect11_h__
//--------------------------------------------------------------------------------
#include "ArkShader11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class PipelineManager;
	class IParameterManager;


	class ArkRenderEffect11
	{
	public:
		ArkRenderEffect11();
		virtual ~ArkRenderEffect11();

		void SetVertexShader( int index );
		

		void SetPixelShader( int index );


		int GetVertexShader( );

		int GetPixelShader( );


		void ConfigurePipeline( PipelineManager* pPipeline, IParameterManager* pParamManager );

		int		m_iBlendState;
		int		m_iDepthStencilState;
		int		m_iRasterizerState;
        UINT    m_uStencilRef; 

	protected:

		void UpdateConstantBufferList( );

		int					m_aiIndices[2];
		ArkShader11*		m_apShaders[2];

		std::vector< std::shared_ptr<ArkRenderParameter11> > m_uniqueConstBuffers;
	};
};


//--------------------------------------------------------------------------------
#endif