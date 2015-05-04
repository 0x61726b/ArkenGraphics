//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewSettings.h
//--------------------------------------------------------------------------------
#ifndef __DepthViewSettings_h__
#define __DepthViewSettings_h__
//--------------------------------------------------------------------------------
#include "ViewSettings.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class DepthViewSettings : public ViewSettings
	{
	public:
		DepthViewSettings(ResourcePtr BackBuffer,ResourcePtr DepthBuffer );
		virtual ~DepthViewSettings();

		
		virtual void SetSettings(const ViewSettings&);

	public:


	};

};
//--------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------