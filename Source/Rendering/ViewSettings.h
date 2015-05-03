//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewSettings.h
//--------------------------------------------------------------------------------
#ifndef __ViewSettings_h__
#define __ViewSettings_h__
//--------------------------------------------------------------------------------
#include "Dx11ResourceProxy.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ViewSettings
	{
	public:
		ViewSettings();
		virtual ~ViewSettings();

		virtual void SetSettings( const ViewSettings& ViewSettings ) = 0;

	public:
		ResourcePtr BackBuffer;
		ResourcePtr DepthBuffer;
	};
	
};
//--------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------