//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewSettings.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "AOViewSettings.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
AOViewSettings::AOViewSettings(ResourcePtr b,ResourcePtr d)
{
	BackBuffer = (b);
	DepthBuffer = (d);
}
//--------------------------------------------------------------------------------
AOViewSettings::~AOViewSettings()
{
}
//--------------------------------------------------------------------------------
void AOViewSettings::SetSettings(const ViewSettings& Settings)
{

}