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
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
RenderTask::RenderTask()
	:m_uiViewportCount(0)
{
}
//--------------------------------------------------------------------------------
RenderTask::~RenderTask()
{
}
//--------------------------------------------------------------------------------
void RenderTask::SetScene( Scene* pScene )
{
	m_pScene = pScene;
}
//--------------------------------------------------------------------------------
void RenderTask::SetViewport(int viewport,unsigned int index)
{
	assert( index < D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE );

	m_iViewports[index] = viewport;
	SetViewportCount(index+1);
}
//--------------------------------------------------------------------------------
void RenderTask::SetViewportCount(unsigned int count)
{
	assert( count < D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE );

	m_uiViewportCount = count;
}
//--------------------------------------------------------------------------------
void RenderTask::ConfigureViewports( PipelineManager* pPipeline )
{
	

	pPipeline->RasterizerStage.State.SetViewportCount( m_uiViewportCount );

	for( int i=0; i < m_uiViewportCount;++i )
	{
		pPipeline->RasterizerStage.State.AddViewport( m_iViewports[i] );
	}
}
//--------------------------------------------------------------------------------
const Dx11ViewPort& RenderTask::GetViewport(unsigned int index)
{
	assert( index < m_uiViewportCount );
	return (ArkRenderer11::Get()->GetViewPort(index) );
}
