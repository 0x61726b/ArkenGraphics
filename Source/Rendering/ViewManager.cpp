//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewManager.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ViewManager.h"
#include "PerspectiveView.h"
#include "ViewAmbientOcclusion.h"
#include "ViewCascadedShadowMap.h"
#include "ViewShadowMap.h"
#include "ViewTextOverlay.h"
#include "ViewOmnidirectionalShadowMap.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ViewManager::ViewManager()
{
}
//--------------------------------------------------------------------------------
ViewManager::~ViewManager()
{
}
//--------------------------------------------------------------------------------
RenderTask* ViewManager::GetView(ArkRenderer11& Renderer,ArkViewTypes type,const ViewSettings& Settings)
{
	if(type == ArkViewTypes::EViewCSM)
	{
		/*PerspectiveView* mView = new*/
		Arkeng::ViewCascadedShadowMap* mView = new Arkeng::ViewCascadedShadowMap(Renderer,Settings);
		return mView;
	}
	if(type == ArkViewTypes::EViewPerspective)
	{
		Arkeng::PerspectiveView* mView = new Arkeng::PerspectiveView(Renderer,Settings);
		return mView;
	}
	if(type == ArkViewTypes::EViewAmbientOcclusion)
	{
		Arkeng::ViewAmbientOcclusion* mView = new Arkeng::ViewAmbientOcclusion(Renderer,Settings);
		return mView;
	}
	if(type == ArkViewTypes::EViewOmnidirectionalShadowMap)
	{
		Arkeng::ViewOmnidirectionalShadowMap* mView = new Arkeng::ViewOmnidirectionalShadowMap(Renderer,Settings);
		return mView;
	}
	return nullptr;
}
//--------------------------------------------------------------------------------
