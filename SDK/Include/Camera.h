//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Camera.h
//--------------------------------------------------------------------------------
#ifndef __Camera_h__
#define __Camera_h__
//--------------------------------------------------------------------------------
#include "Actor.h"
#include "RenderTask.h"
#include "IEventListener.h"
#include "ArkSpatialController.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Scene;

	class Camera : public Actor,public IEventListener
	{
	public:
		Camera();
		virtual ~Camera();


		void SetCameraView(RenderTask*);

		RenderTask*		GetCameraView();

		void RenderFrame(ArkRenderer11* Renderer);

		void SetScene(Scene* pScene);

		void ApplyProjectionParams();
		void ApplyOrthographicParams();

		void SetProjectionParams(float zn,float zf,float aspect,float fov);
		void SetOrthographicParams(float zn,float zf,float width,float height);

		void SetClipPlanes(float zn,float zf);
		void SetAspectRatio(float aspect);
		void SetFieldOfView(float fov);

		float GetNearClipPlane();
		float GetFarClipPlane();
		float GetAspectRatio();
		float GetFieldOfView();

		float m_fNear;
		float m_fFar;
		float m_fAspect;
		float m_fFov;

		float m_fWidth;
		float m_fHeight;

		virtual bool HandleEvent(EventPtr pEvent);
		virtual std::wstring GetName();

		ArkSpatialController<ArkNode3D>& Spatial(); 

		DirectX::XMMATRIX m_ProjMatrix;
	protected:
		RenderTask*			m_pCameraView;
		Scene*				m_pScene;

		ArkSpatialController<ArkNode3D>*	m_pSpatialController;

	};
};



//--------------------------------------------------------------------------------
#endif
