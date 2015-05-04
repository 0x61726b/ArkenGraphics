//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewSettings.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "DepthViewSettings.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
DepthViewSettings::DepthViewSettings(ResourcePtr b,ResourcePtr d)
{
	BackBuffer = (b);
	DepthBuffer = (d);
}
//--------------------------------------------------------------------------------
DepthViewSettings::~DepthViewSettings()
{
}
//--------------------------------------------------------------------------------
void DepthViewSettings::SetSettings(const ViewSettings& Settings)
{

}