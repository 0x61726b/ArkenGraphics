//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkNodedGeometryExecutor11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkNodedGeometryExecutor11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkNodedGeometryExecutor11::ArkNodedGeometryExecutor11()
	: m_pMaterial(NULL)
{
}
//--------------------------------------------------------------------------------
ArkNodedGeometryExecutor11::~ArkNodedGeometryExecutor11()
{
}
//--------------------------------------------------------------------------------
void ArkNodedGeometryExecutor11::Execute(PipelineManager* pPipeline,IParameterManager* pParamManager)
{
	for(int i=0; i < Root->Children.size(); i++)
	{
		Root->Children[i]->Execute(pPipeline,pParamManager);
	}
}
//--------------------------------------------------------------------------------
void ArkNodedGeometryExecutor11::SetLayoutElements(unsigned int count,D3D11_INPUT_ELEMENT_DESC* pElements)
{
	for(int i=0; i < Root->Children.size(); i++)
	{
		Root->Children[i]->SetLayoutElements(count,pElements);
	}
}
//--------------------------------------------------------------------------------
void ArkNodedGeometryExecutor11::GenerateInputLayout(int ShaderID)
{
#ifdef __OPTIMIZED_INPUT_LAYOUT__
	ArkGeometry11* Child = Root->Children[0];
	bool success = Child->ComputeTangentFrame();
	_ASSERT(success);
	if(Child)
	{
		Child->GenerateInputLayout(ShaderID);

		for(int i=1; i < Root->Children.size(); i++)
		{
			Root->Children[i]->m_InputLayouts = Child->m_InputLayouts;
		}
	}
#else
	for(int i=0; i < Root->Children.size(); i++)
	{
		Root->Children[i]->GenerateInputLayout(ShaderID);
	}
#endif
}
//--------------------------------------------------------------------------------
void ArkNodedGeometryExecutor11::LoadToBuffers()
{
	
	for(int i=0; i < Root->Children.size(); i++)
	{
		Root->Children[i]->CalculateMaterialIDs();
		Root->Children[i]->LoadToBuffers();
	}
}
//--------------------------------------------------------------------------------
ArkMaterial11& ArkNodedGeometryExecutor11::Material() const
{
	return *m_pMaterial;
}
//--------------------------------------------------------------------------------
void ArkNodedGeometryExecutor11::SetMaterial(ArkMaterial11* p)
{
	//m_pMaterial = p;

	//for(int i=0; i < Root->Children.size(); i++)
	//{
	//	Root->Children[i]->Material = m_pMaterial;
	//}
}
//--------------------------------------------------------------------------------