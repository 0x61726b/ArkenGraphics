//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkDrawExecutor11.h
//--------------------------------------------------------------------------------
#ifndef __ArkDrawExecutor11_h__
#define __ArkDrawExecutor11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "PipelineExecutor11.h"
#include "TGrowableVertexBuffer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	template <class TVertex>
	class ArkDrawExecutor11 : public PipelineExecutor11
	{
	public:
		ArkDrawExecutor11( );
		virtual ~ArkDrawExecutor11( );
	
		virtual void Execute( PipelineManager* pPipeline, IParameterManager* pParamManager );
		virtual void ResetGeometry();
		virtual void ResetVertices();

		void AddVertex( const TVertex& vertex );

		D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType();
		void SetPrimitiveType( D3D11_PRIMITIVE_TOPOLOGY type );

		unsigned int GetVertexCount();
		virtual unsigned int GetPrimitiveCount();

		void SetMaxVertexCount( unsigned int count );

	protected:

		// The type of primitives listed in the index buffer
		D3D11_PRIMITIVE_TOPOLOGY m_ePrimType;

		// Use a growable vertex buffer to hold the vertex data.
		TGrowableVertexBuffer11<TVertex> VertexBuffer;
	};
	#include "ArkDrawExecutor11.inl"
};
//--------------------------------------------------------------------------------
#endif