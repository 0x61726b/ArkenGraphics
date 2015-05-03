//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewSettings.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "CSMViewSettings.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
CSMViewSettings::CSMViewSettings(ResourcePtr b,ResourcePtr d)
	: CascadeCount(DEFAULT_CSM_CASCADE_COUNT),
	ShadowMapSize(DEFAULT_CSM_SM_SIZE),
	ShadowDist(1.0f),
	Backup(20),
	NearClip(40),
	CascadeSplits(),
	Bias(0.005f)
{
	float c[4] = { 0.125f,0.25f,0.5f,1.0f };
	CascadeSplits = c;

	BackBuffer = (b);
	DepthBuffer = (d);
}
//--------------------------------------------------------------------------------
CSMViewSettings::~CSMViewSettings()
{
}
//--------------------------------------------------------------------------------
void CSMViewSettings::SetSettings(const ViewSettings& Settings)
{

}