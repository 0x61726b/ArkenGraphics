//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkGeometryNode11.h
//--------------------------------------------------------------------------------
#ifndef __ArkGeometryNode11_h__
#define __ArkGeometryNode11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkGeometry11.h"
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
	struct NodeProperties
	{
		std::wstring					NodeName;
		ResourcePtr						DiffuseMap;
		ResourcePtr						BumpMap;
		std::vector<TriangleIndices>	Faces;
		std::vector<XMFLOAT3>			Points;
		std::vector<XMFLOAT2>			UVs;
		std::vector<XMFLOAT3>			Normals;
		std::vector<XMFLOAT3>			Tangents;
		std::vector<XMFLOAT3>			Binormals;

		NodeProperties():
			NodeName(std::wstring(L"ArkSceneRoot"))
		{

		}
	};
	class ArkVertexBuffer11;
	class ArkIndexBuffer11;


	class ArkGeometryNode11 : public PipelineExecutor11
	{
	public:
		ArkGeometryNode11();
		virtual ~ArkGeometryNode11();

		std::vector<ArkGeometryNode11> Children;

		virtual void Execute(PipelineManager* pPipeline,IParameterManager* pParamManager);

		void AddElement(VertexElement11* element);
		void AddFace(TriangleIndices& face);
		void AddLine(LineIndices& line);
		void AddPoint(PointIndices& point);
		void AddIndex(UINT index);

		VertexElement11* GetElement(std::string name);
		VertexElement11* GetElement(std::wstring name);
		VertexElement11* GetElement(int index);

		void GenerateInputLayout(int ShaderID);

		void LoadToBuffers();

		UINT GetIndex(unsigned int index);

		D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType();
		void SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY type);

		int GetPrimitiveCount();
		UINT GetIndexCount();

		int GetVertexCount();
		int GetElementCount();
		int GetVertexSize();

		int CalculateVertexSize();
		int CalculateVertexCount();

		bool ComputeTangentFrame(std::string positionSemantic = VertexElement11::PositionSemantic,
			std::string normalSemantic = VertexElement11::NormalSemantic,
			std::string texCoordSemantic = VertexElement11::TexCoordSemantic,
			std::string tangentSemantic = VertexElement11::TangentSemantic);

		std::vector<VertexElement11*>		m_vElements;
		std::vector<UINT>					m_vIndices;

		// The size 
		int m_iVertexSize;
		int m_iVertexCount;

		ResourcePtr m_VB;
		ResourcePtr m_IB;


		// The type of primitives listed in the index buffer
		D3D11_PRIMITIVE_TOPOLOGY m_ePrimType;
	public:
		NodeProperties*		NodeProps;
		ArkGeometryNode11*   Parent;
	};
	typedef std::shared_ptr<ArkGeometryNode11> GeometryNodePtr;
}
//--------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------