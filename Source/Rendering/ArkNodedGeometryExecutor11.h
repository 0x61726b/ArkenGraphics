//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkNodedGeometryExecutor11.h
//--------------------------------------------------------------------------------
#ifndef __ArkNodedGeometryExecutor11_h__
#define __ArkNodedGeometryExecutor11_h__
//--------------------------------------------------------------------------------
#include "ArkGeometry11.h"
#include "PipelineExecutor11.h"

#define __OPTIMIZED_INPUT_LAYOUT__
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkMaterial11;

	class ArkNodedGeometryExecutor11 : public PipelineExecutor11
	{
	public:
		ArkNodedGeometryExecutor11();
		virtual ~ArkNodedGeometryExecutor11();

		virtual void Execute(PipelineManager* pPipeline,IParameterManager* pParamManager);
		virtual void SetLayoutElements(unsigned int count,D3D11_INPUT_ELEMENT_DESC* pElements);
		virtual void GenerateInputLayout(int ShaderID);

		void LoadToBuffers();

		ArkMaterial11& Material() const;
		void SetMaterial(ArkMaterial11*);

		ArkMaterial11* m_pMaterial;

		ArkGeometry11* Root;
	};
	typedef std::shared_ptr<ArkNodedGeometryExecutor11> NodedGeometryPtr;
}
//--------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------