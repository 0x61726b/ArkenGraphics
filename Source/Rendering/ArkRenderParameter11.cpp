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
	for ( int i = 0; i < NUM_THREADS+1; i++ ) {
		m_auiValueID[i] = 0;
	}
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
//--------------------------------------------------------------------------------
unsigned int ArkRenderParameter11::GetValueID(unsigned int threadID)
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );


	return( m_auiValueID[threadID] );
}