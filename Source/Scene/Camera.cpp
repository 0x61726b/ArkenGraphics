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
#include "ArkLog.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Camera::Camera()
	: m_pCameraView(nullptr),
	m_pScene(nullptr),
	m_pOverlayView(nullptr),
	m_fNear(0.01f),
	m_fFar(100.0f),
	m_fAspect(1280.0f / 800.0f),
	m_fFov(DirectX::XM_PIDIV2),
	m_fWidth(1280.0f),
	m_fHeight(800.0f),
	m_ProjMatrix(DirectX::XMMatrixIdentity()),
	m_pSpatialController(nullptr)
{
	m_pSpatialController = new ArkSpatialController<ArkNode3D>();
	GetNode()->Controllers.Attach(m_pSpatialController);

	m_pViewPositionWriter = Parameters.SetVectorParameter(L"ViewPosition",XMVectorSet(0.0f,0.0f,0.0f,0.0f));
}
//--------------------------------------------------------------------------------
Camera::~Camera()
{
	Safe_Delete(m_pCameraView);
	Safe_Delete(m_pOverlayView);
}
//--------------------------------------------------------------------------------
void Camera::SetOverlayView( TaskCore* pTask )
{
	m_pOverlayView = pTask;
}
//--------------------------------------------------------------------------------
void Camera::RenderFrame(ArkRenderer11* pRenderer)
{

	if( m_pOverlayView )
		pRenderer->QueueTask( m_pOverlayView );

	if(m_pCameraView)
	{
		if(m_pScene)
		{
			m_pCameraView->SetScene(m_pScene);
		}

		//

		//// Set the view position in the parameter system, for use by any of the
		//// views being used in this frame.

		XMVECTOR p = GetNode()->Transform.Position() + GetBody()->Transform.Position();
		XMFLOAT3 pp;

		m_pViewPositionWriter->SetValue(p);

		Parameters.InitRenderParams();

		//// Use the pre-draw method to queue any needed render views in the scene.
		//// This is followed by rendering all of the views to generate the current
		//// frame.

		m_pCameraView->QueuePreTasks(pRenderer);
		pRenderer->ProcessTaskQueue();


	}
}
//--------------------------------------------------------------------------------
bool Camera::HandleEvent(EventPtr pEvent)
{
	eEvent e = pEvent->GetEventType();

	return false;
}
//--------------------------------------------------------------------------------
std::wstring Camera::GetName()
{
	return L"Camera";
}
//--------------------------------------------------------------------------------
void Camera::SetCameraView(RenderTask* pView)
{
	m_pCameraView = pView;
	m_pCameraView->SetEntity(m_pBody);
}
//--------------------------------------------------------------------------------
RenderTask* Camera::GetCameraView()
{
	return m_pCameraView;
}
//--------------------------------------------------------------------------------
void Camera::SetScene(Scene* pScene)
{
	m_pScene = pScene;
}
//--------------------------------------------------------------------------------
void Camera::ApplyProjectionParams()
{
	m_ProjMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2,m_fWidth/m_fHeight,m_fNear,m_fFar);




	if(m_pCameraView)
		m_pCameraView->SetProjMatrix(m_ProjMatrix);
}
//--------------------------------------------------------------------------------
void Camera::ApplyOrthographicParams()
{
	m_ProjMatrix = DirectX::XMMatrixOrthographicLH(m_fWidth,m_fHeight,m_fNear,m_fFar );

	if( m_pCameraView)
		m_pCameraView->SetProjMatrix(m_ProjMatrix);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Camera::SetProjectionParams(float zn,float zf,float aspect,float fov)
{
	m_fNear = zn;
	m_fFar = zf;
	m_fAspect = aspect;
	m_fFov = fov;

	ApplyProjectionParams();
}
//--------------------------------------------------------------------------------
void Camera::SetOrthographicParams(float zn,float zf,float width,float height)
{
	m_fNear = zn;
	m_fFar = zf;
	m_fWidth = width;
	m_fHeight = height;

	ApplyOrthographicParams();
}
//--------------------------------------------------------------------------------
void Camera::SetClipPlanes(float zn,float zf)
{
	m_fNear = zn;
	m_fFar = zf;

	ApplyProjectionParams();
}
//--------------------------------------------------------------------------------
void Camera::SetAspectRatio(float aspect)
{
	m_fAspect = aspect;

	ApplyProjectionParams();
}
//--------------------------------------------------------------------------------
void Camera::SetFieldOfView(float fov)
{
	m_fFov = fov;

	ApplyProjectionParams();
}
//--------------------------------------------------------------------------------
float Camera::GetNearClipPlane()
{
	return(m_fNear);
}
//--------------------------------------------------------------------------------
float Camera::GetFarClipPlane()
{
	return(m_fFar);
}
//--------------------------------------------------------------------------------
float Camera::GetAspectRatio()
{
	return(m_fAspect);
}
//--------------------------------------------------------------------------------
float Camera::GetFieldOfView()
{
	return(m_fFov);
}
//--------------------------------------------------------------------------------
ArkSpatialController<ArkNode3D>& Camera::Spatial()
{
	return(*m_pSpatialController);
}
//--------------------------------------------------------------------------------