//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkGeometry11.h
//--------------------------------------------------------------------------------
#ifndef __ArkGeometry11_h__
#define __ArkGeometry11_h__
//--------------------------------------------------------------------------------
#include "PCH.h"
#include "Dx11ResourceProxy.h"
#include "VertexElement11.h"
#include "TriangleIndices.h"
#include "LineIndices.h"
#include "PointIndices.h"
#include "PipelineExecutor11.h"
#include "DxIAState.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkVertexBuffer11;
	class ArkIndexBuffer11;

	class ArkGeometry11 : public PipelineExecutor11
	{
	public:
		ArkGeometry11( );
		virtual ~ArkGeometry11( );
	
		virtual void Execute( PipelineManager* pPipeline, IParameterManager* pParamManager );

		void AddElement( VertexElement11* element );
		void AddFace( TriangleIndices& face );
		void AddLine( LineIndices& line );
		void AddPoint( PointIndices& point );
		void AddIndex( UINT index );

        VertexElement11* GetElement( std::string name );
		VertexElement11* GetElement( std::wstring name );
		VertexElement11* GetElement( int index );

		UINT GetIndex( unsigned int index );

		D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType();
		void SetPrimitiveType( D3D11_PRIMITIVE_TOPOLOGY type );

		int GetPrimitiveCount();
		UINT GetIndexCount();
		
		int GetVertexCount();
		int GetElementCount();
		int GetVertexSize();

		int CalculateVertexSize();
		int CalculateVertexCount();

		void GenerateInputLayout( int ShaderID );

		void LoadToBuffers( );

        bool ComputeTangentFrame( std::string positionSemantic = VertexElement11::PositionSemantic,
                                  std::string normalSemantic = VertexElement11::NormalSemantic,
                                  std::string texCoordSemantic = VertexElement11::TexCoordSemantic, 
                                  std::string tangentSemantic = VertexElement11::TangentSemantic );

		std::vector<VertexElement11*>		m_vElements;
		std::vector<UINT>					m_vIndices;
		
		ResourcePtr m_VB;
		ResourcePtr m_IB;

		// The size 
		int m_iVertexSize;
		int m_iVertexCount;

		// The type of primitives listed in the index buffer
		D3D11_PRIMITIVE_TOPOLOGY m_ePrimType;
	};

	typedef std::shared_ptr<ArkGeometry11> GeometryPtr;
};
//--------------------------------------------------------------------------------
#endif // GeometryDX11_h
