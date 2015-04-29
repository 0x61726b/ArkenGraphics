//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------
#ifndef __ArkRenderable_h__
#define __ArkRenderable_h__
//--------------------------------------------------------------------------------
#include "PipelineExecutor11.h"
#include "ArkMaterial11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	// The render pass is used to allow an object to define what type of
	// object it actually is.  This can then be used by a render view to
	// provide a special consideration when selecting its object rendering 
	// order.

	class ArkRenderable
	{
	public:
		enum ENTITYTYPE
		{
			Geometry,
		};

		ArkRenderable();
		~ArkRenderable();

		void SetMaterial( MaterialPtr pMaterial );
		MaterialPtr GetMaterial( );

		void SetGeometry( ExecutorPtr pExecutor );
		ExecutorPtr GetGeometry( );

		
		ENTITYTYPE				iPass;
		ExecutorPtr				Executor;
		MaterialPtr				Material;
	};
};
//--------------------------------------------------------------------------------
#endif // Renderable_h
//--------------------------------------------------------------------------------