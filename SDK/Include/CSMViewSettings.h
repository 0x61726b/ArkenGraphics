//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewSettings.h
//--------------------------------------------------------------------------------
#ifndef __CSMViewSettings_h__
#define __CSMViewSettings_h__
//--------------------------------------------------------------------------------
#include "ViewSettings.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class CSMViewSettings : public ViewSettings
	{
	public:
		CSMViewSettings(ResourcePtr BackBuffer,ResourcePtr DepthBuffer );
		virtual ~CSMViewSettings();

		
		virtual void SetSettings(const ViewSettings&);

	public:
		const UINT			CascadeCount;
		const UINT			ShadowMapSize;
		const float			ShadowDist;
		const float			Backup;
		const float			NearClip;
		const float*		CascadeSplits;
		const float			Bias;

	};

};
//--------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------