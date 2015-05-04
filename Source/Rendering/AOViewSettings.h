//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewSettings.h
//--------------------------------------------------------------------------------
#ifndef __AOViewSettings_h__
#define __AOViewSettings_h__
//--------------------------------------------------------------------------------
#include "ViewSettings.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class AOViewSettings : public ViewSettings
	{
	public:
		AOViewSettings(ResourcePtr BackBuffer,ResourcePtr DepthBuffer );
		virtual ~AOViewSettings();

		
		virtual void SetSettings(const ViewSettings&);

	public:


	};

};
//--------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------