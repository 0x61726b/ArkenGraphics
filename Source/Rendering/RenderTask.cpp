//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RenderTask.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "RenderTask.h"
#include "IParameterManager.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
RenderTask::RenderTask()
	:	m_uiViewportCount(0),
		ViewMatrix(),
		ProjMatrix(),
		m_pEntity( nullptr ),
		m_pScene( nullptr ),
		m_iViewports()
{
}
//--------------------------------------------------------------------------------
RenderTask::~RenderTask()
{
}
//--------------------------------------------------------------------------------
void RenderTask::SetScene(Scene* pScene)
{
	m_pScene = pScene;
}
//--------------------------------------------------------------------------------
void RenderTask::SetViewport(int viewport,unsigned int index)
{
	assert(index < D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE);

	m_iViewports[index] = viewport;
	SetViewportCount(index+1);
}
//--------------------------------------------------------------------------------
void RenderTask::SetViewportCount(unsigned int count)
{
	assert(count < D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE);

	m_uiViewportCount = count;
}
//--------------------------------------------------------------------------------
void RenderTask::ConfigureViewports(PipelineManager* pPipeline)
{


	pPipeline->RasterizerStage.CurrentState.ViewPortCount.SetState(m_uiViewportCount);

	for(int i=0; i < m_uiViewportCount;++i)
	{
		pPipeline->RasterizerStage.CurrentState.ViewPorts.SetState(0,m_iViewports[i]);
	}
}
//--------------------------------------------------------------------------------
const Dx11ViewPort& RenderTask::GetViewport(unsigned int index)
{
	assert(index < m_uiViewportCount);
	return (ArkRenderer11::Get()->GetViewPort(index));
}
//--------------------------------------------------------------------------------
void RenderTask::SetProjMatrix(const DirectX::XMMATRIX& v)
{
	ProjMatrix = v;
}
//--------------------------------------------------------------------------------
void RenderTask::SetViewMatrix(const DirectX::XMMATRIX& v)
{
	ViewMatrix = v;
}
//--------------------------------------------------------------------------------
void RenderTask::SetRenderParams(IParameterManager* pParamManager)
{
	pParamManager->SetViewMatrix(&ViewMatrix);
	pParamManager->SetProjectionMatrix(&ProjMatrix);
}
//--------------------------------------------------------------------------------
void RenderTask::SetUsageParams(IParameterManager* pParamManager)
{
}
//--------------------------------------------------------------------------------
const DirectX::XMMATRIX& RenderTask::GetViewMatrix()
{
	return ViewMatrix;
}
//--------------------------------------------------------------------------------
const DirectX::XMMATRIX& RenderTask::GetProjMatrix()
{
	return ProjMatrix;
}
//--------------------------------------------------------------------------------