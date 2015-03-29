//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkTimer.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkTimer.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkTimer::ArkTimer()
{
	m_iFramesPerSecond = 0;
	m_iMaxFramesPerSecond = 0;
	m_iFrameCount = 0;

	m_fDelta = 0;
	m_fFixedDelta = 0.0f;
	m_bUseFixedStep = false;

	QueryPerformanceFrequency((LARGE_INTEGER*)&m_TicksPerSecond64);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrentTicks64);
	m_StartupTicks64 = m_CurrentTicks64;
	m_OneSecTicks64 = m_CurrentTicks64;
}
//--------------------------------------------------------------------------------
ArkTimer::~ArkTimer()
{
}
//--------------------------------------------------------------------------------
void ArkTimer::Reset()
{
	m_iFramesPerSecond = 0;
	m_iFrameCount = 0;
	m_fDelta = 0;
}
//--------------------------------------------------------------------------------
void ArkTimer::SetFixedTimeStep( float step )
{
	if ( step <= 0.0f )
	{
		m_bUseFixedStep = false;
		m_fFixedDelta = 0.0f;
	}
	else
	{
		m_bUseFixedStep = true;
		m_fFixedDelta = step;
	}
}
//--------------------------------------------------------------------------------
void ArkTimer::Update( )
{
	m_LastTicks64 = m_CurrentTicks64;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrentTicks64);

	// Update the time increment
	
	if ( m_bUseFixedStep )
		m_fDelta = m_fFixedDelta;
	else
		m_fDelta = (float)((__int64)m_CurrentTicks64 - (__int64)m_LastTicks64) / (__int64)m_TicksPerSecond64;

	// Continue counting the frame rate regardless of the time step.

	if ((float)((__int64)m_CurrentTicks64 - (__int64)m_OneSecTicks64)
		/ (__int64)m_TicksPerSecond64 < 1.0f)
	{
		m_iFrameCount++;
	}
	else
	{
		m_iFramesPerSecond = m_iFrameCount;
		
		if ( m_iFramesPerSecond > m_iMaxFramesPerSecond )
			m_iMaxFramesPerSecond = m_iFramesPerSecond;

		m_iFrameCount = 0;
		m_OneSecTicks64 = m_CurrentTicks64;
	}

}
//--------------------------------------------------------------------------------
float ArkTimer::Elapsed( )
{
	return( m_fDelta );
}
//--------------------------------------------------------------------------------
int ArkTimer::Framerate( )
{
	return( m_iFramesPerSecond );
}
//--------------------------------------------------------------------------------
float ArkTimer::Runtime( )
{
	return ( (float)((__int64)m_CurrentTicks64 - (__int64)m_StartupTicks64)
		/(__int64)m_TicksPerSecond64);
}
//--------------------------------------------------------------------------------
int ArkTimer::MaxFramerate()
{
	return( m_iMaxFramesPerSecond );
}
//--------------------------------------------------------------------------------
int ArkTimer::FrameCount()
{
	return( m_iFrameCount );
}
//--------------------------------------------------------------------------------