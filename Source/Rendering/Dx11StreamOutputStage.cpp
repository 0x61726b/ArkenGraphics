//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11StreamOutputStage11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11StreamOutputStage.h"
#include "ArkVertexBuffer11.h"
#include "ArkLog.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11StreamOutputStage::Dx11StreamOutputStage()
{
	CurrentState.SetSisterState( &PreviousState );
}
//--------------------------------------------------------------------------------
Dx11StreamOutputStage::~Dx11StreamOutputStage()
{
}
//--------------------------------------------------------------------------------
void Dx11StreamOutputStage::SetFeautureLevel( D3D_FEATURE_LEVEL level )
{
	m_FeatureLevel = level;
	PreviousState.SetFeautureLevel( level );
	CurrentState.SetFeautureLevel( level );
}
//--------------------------------------------------------------------------------
void Dx11StreamOutputStage::ClearCurrentState( )
{
	CurrentState.ClearState();
}
//--------------------------------------------------------------------------------
void Dx11StreamOutputStage::ClearPreviousState( )
{
	PreviousState.ClearState();
}
//--------------------------------------------------------------------------------
void Dx11StreamOutputStage::ApplyDesiredState( ID3D11DeviceContext* pContext )
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();

	// Bind the vertex buffers
	if ( CurrentState.StreamBuffers.IsUpdateNeeded()
		|| CurrentState.StreamOffsets.IsUpdateNeeded() )
	{
		ID3D11Buffer* Buffers[4] = { NULL };

		for ( unsigned int i = 0; i < sizeof( Buffers ) / sizeof( ID3D11Buffer* ); i++ )
		{
			int index = CurrentState.StreamBuffers.GetState( i );

			std::shared_ptr<ArkVertexBuffer11> pBuffer = pRenderer->GetVertexBufferByIndex( index );

			if ( pBuffer ) {
				Buffers[i] = static_cast<ID3D11Buffer*>( pBuffer->GetResource() );
			} else {
				Buffers[i] = 0;
			}
		}

		// Start slot is always zero since there is no way to specify buffers
		// starting from a different index.

		UINT startSlot = 0;

		UINT endSlot = max( CurrentState.StreamBuffers.GetEndSlot(),
			CurrentState.StreamOffsets.GetEndSlot() );

		pContext->SOSetTargets( 
			endSlot-startSlot+1, 
			Buffers,
			CurrentState.StreamOffsets.GetSlotLocation( 0 ) );
	}

	CurrentState.ResetUpdateFlags();
	PreviousState = CurrentState;
}
