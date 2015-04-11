//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkComputeShaderStage11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11StreamOutputStageState.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11StreamOutputStageState::Dx11StreamOutputStageState() :
	StreamBuffers( -1 ),
	StreamOffsets( 0 ),
	m_FeatureLevel( D3D_FEATURE_LEVEL_9_1 ),
	m_pSisterState( nullptr )
{
	ClearState();
}
//--------------------------------------------------------------------------------
Dx11StreamOutputStageState::~Dx11StreamOutputStageState()
{
	
}
//--------------------------------------------------------------------------------
void Dx11StreamOutputStageState::SetFeautureLevel( D3D_FEATURE_LEVEL level )
{
	m_FeatureLevel = level;
}
//--------------------------------------------------------------------------------
int Dx11StreamOutputStageState::GetBufferCount() const
{
	unsigned int count = 0;

	for ( unsigned int i = 0; i < 4; i++ )
	{
		if ( StreamBuffers.GetState( i ) != -1 )
			count++;
	}

	return( count );
}
//--------------------------------------------------------------------------------
void Dx11StreamOutputStageState::ClearState( )
{
	StreamBuffers.InitializeStates();
	StreamOffsets.InitializeStates();
}
//--------------------------------------------------------------------------------
void Dx11StreamOutputStageState::SetSisterState( Dx11StreamOutputStageState* pState )
{
	m_pSisterState = pState;

	StreamBuffers.SetSister( &m_pSisterState->StreamBuffers );
	StreamOffsets.SetSister( &m_pSisterState->StreamOffsets );
}
//--------------------------------------------------------------------------------
void Dx11StreamOutputStageState::ResetUpdateFlags( )
{
	StreamBuffers.ResetTracking();
	StreamOffsets.ResetTracking();
}
//--------------------------------------------------------------------------------
