//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//X.h
//--------------------------------------------------------------------------------
#ifndef __ArkMaterial11_h__
#define __ArkMaterial11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkRenderer11.h"
#include "TaskCore.h"
#include "ArkRenderEffect11.h"
#include "ArkRenderParameter11.h"
#include "ArkParameterContainer.h"
#include "RenderTask.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	struct MaterialParams
	{
		bool										bRender;
		bool										bCastShadow;
		std::shared_ptr<ArkRenderEffect11>			pEffect;
		std::vector<TaskCore*>						Tasks;
	};

	class ArkMaterial11
	{
	public:
		ArkMaterial11();
		virtual ~ArkMaterial11();

		void Update( float time );
		void PreRender( ArkRenderer11* pRenderer, VIEWTYPE type );
		void SetRenderParams( IParameterManager* pParamManager, VIEWTYPE type );
		
		// This method should return *all* possible vertex shader IDs that could
		// be used by this material.  This is used to generate the input layouts
		// to be used with them.
		void GetAllVertexShaderIDs( std::vector<int>& idlist );

	public:
		MaterialParams					Params[VT_NUM_VIEW_TYPES];
		ArkParameterContainer			Parameters;
	};
	typedef std::shared_ptr<ArkMaterial11> MaterialPtr;
};
//--------------------------------------------------------------------------------
#endif // MaterialDX11_h
//--------------------------------------------------------------------------------

