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
}
//--------------------------------------------------------------------------------
void Camera::RenderFrame(ArkRenderer11* pRenderer)
{
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

		//XMFLOAT4X4 mm;
		//XMStoreFloat4x4(&mm,m_ProjMatrix);

		//std::wstring m11 = std::to_wstring(mm._11);
		//std::wstring m12 = std::to_wstring(mm._12);
		//std::wstring m13 = std::to_wstring(mm._13);
		//std::wstring m14 = std::to_wstring(mm._14);

		//std::wstring m21 = std::to_wstring(mm._21);
		//std::wstring m22 = std::to_wstring(mm._22);
		//std::wstring m23 = std::to_wstring(mm._23);
		//std::wstring m24 = std::to_wstring(mm._24);

		//std::wstring m31 = std::to_wstring(mm._31);
		//std::wstring m32 = std::to_wstring(mm._32);
		//std::wstring m33 = std::to_wstring(mm._33);
		//std::wstring m34 = std::to_wstring(mm._34);

		//std::wstring m41 = std::to_wstring(mm._41);
		//std::wstring m42 = std::to_wstring(mm._42);
		//std::wstring m43 = std::to_wstring(mm._43);
		//std::wstring m44 = std::to_wstring(mm._44);


		//std::wstring projString = L"\n";
		//projString.append(m11 + L" ");
		//projString.append(m12 + L" ");
		//projString.append(m13 + L" ");
		//projString.append(m14 + L" ");
		//projString.append(L"\n");

		//projString.append(m21 + L" ");
		//projString.append(m22 + L" ");
		//projString.append(m23 + L" ");
		//projString.append(m24 + L" ");
		//projString.append(L"\n");

		//projString.append(m31 + L" ");
		//projString.append(m32 + L" ");
		//projString.append(m33 + L" ");
		//projString.append(m34 + L" ");
		//projString.append(L"\n");

		//projString.append(m41 + L" ");
		//projString.append(m42 + L" ");
		//projString.append(m43 + L" ");
		//projString.append(m44 + L" ");
		//projString.append(L"\n");


		//ArkLog::Get(LogType::Renderer).Output(projString);

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
	//
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