//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Camera.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Camera.h"
#include "Scene.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Camera::Camera() 
	: m_vCameraView( nullptr )
{
}
//--------------------------------------------------------------------------------
Camera::~Camera()
{
	Safe_Delete( m_vCameraView );
}
//--------------------------------------------------------------------------------
void Camera::RenderFrame( ArkRenderer11* pRenderer )
{
	if( m_vCameraView )
	{
		if( m_pScene )
		{
			m_vCameraView->SetScene( m_pScene );
		}
		m_vCameraView->QueuePreTasks( pRenderer );
		pRenderer->ProcessTaskQueue();

	}
}
//--------------------------------------------------------------------------------
void Camera::SetCameraView( RenderTask* pView )
{
	m_vCameraView = pView;
}
//--------------------------------------------------------------------------------
RenderTask* Camera::GetCameraView()
{
	return m_vCameraView;
}
//--------------------------------------------------------------------------------
void Camera::SetScene( Scene* pScene )
{
	m_pScene = pScene;
}

