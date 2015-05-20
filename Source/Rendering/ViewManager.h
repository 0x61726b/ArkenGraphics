//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewManager.h
//--------------------------------------------------------------------------------
#ifndef __ViewManager_h__
#define __ViewManager_h__
//--------------------------------------------------------------------------------
#include "RenderTask.h"
#include "ViewSettings.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	enum ArkViewTypes
	{
		EViewPerspective,
		EViewCSM,
		EViewShadowMap,
		EViewTextOverlay,
		EViewAmbientOcclusion,
		EViewOmnidirectionalShadowMap,
		EViewWireframe
	};
	class ViewManager
	{
	public:
		ViewManager();
		~ViewManager();

		RenderTask* GetView( ArkRenderer11& Renderer, ArkViewTypes view,const ViewSettings& ViewSetting );

	};
};
//--------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------