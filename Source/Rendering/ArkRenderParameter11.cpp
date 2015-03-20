//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkRenderParameter11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkRenderParameter11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkRenderParameter11::ArkRenderParameter11()
{
}
//--------------------------------------------------------------------------------
ArkRenderParameter11::ArkRenderParameter11(ArkRenderParameter11& copy)
{
	m_sParameterName = copy.m_sParameterName;
}
//--------------------------------------------------------------------------------
ArkRenderParameter11::~ArkRenderParameter11()
{
}
//--------------------------------------------------------------------------------
void ArkRenderParameter11::SetName(const std::wstring& name)
{
	m_sParameterName = name;
}
//--------------------------------------------------------------------------------
std::wstring& ArkRenderParameter11::GetName()
{
	return m_sParameterName;
}
//--------------------------------------------------------------------------------
void ArkRenderParameter11::InitializeParameterData(void* pData)
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
		SetParameterData( pData, i );
}